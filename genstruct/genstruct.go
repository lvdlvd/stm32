package main

import (
	"encoding/json"
	"encoding/xml"
	"flag"
	"fmt"
	"log"
	"os"
	"path/filepath"
	"sort"
	"strconv"
	"strings"
	"text/template"
	"unicode"
)

type Device struct {
	Name            string        `xml:"name"`
	Version         string        `xml:"version"`
	Description     string        `xml:"description"`
	AddressUnitBits number        `xml:"addressUnitBits"`
	CpuName         string        `xml:"cpu>name"`
	Width           number        `xml:"width"`
	Size            number        `xml:"size"`       // peripheral register default
	ResetValue      number        `xml:"resetValue"` // peripheral register default
	ResetMask       number        `xml:"resetMask"`  // peripheral register default
	Peripherals     []*Peripheral `xml:"peripherals>peripheral"`
	Interrupts      []*Interrupt  `xml:"interrupt"` // global as well as created from the peripherals
}

func (d *Device) PeripheralByName(s string) *Peripheral {
	for _, v := range d.Peripherals {
		if v.Name == s {
			return v
		}
	}
	return nil
}

func (d *Device) PeripheralType(s string) *Peripheral {
	p := d.PeripheralByName(s)
	if p.DerivedFrom != "" {
		return d.PeripheralType(p.DerivedFrom)
	}
	return p
}

var renameMap = map[string]string{}

func (d *Device) TypeName(s string) string {
	n := d.PeripheralType(s)
	if n == nil {
		return "<NONE>"
	}
	if m, ok := renameMap[n.Name]; ok {
		return m
	}
	return n.Name
}

type Peripheral struct {
	Name             string        `xml:"name"`
	DerivedFrom      string        `xml:"derivedFrom,attr"` // same type as this device
	Description      string        `xml:"description"`
	GroupName        string        `xml:"groupName"`
	BaseAddress      number        `xml:"baseAddress"`
	AddressBlockSize number        `xml:"addressBlock>Size"`
	Interrupts       []*Interrupt  `xml:"interrupt"`
	Registers        []*Register   `xml:"registers>register"`
	Extends          []*Peripheral `xml:-` // list of base types (registers and bits are strict subset)
	Others           []*Peripheral `xml:-` // list of types that have this one as 'DerivedFrom' value
}

func (p *Peripheral) RegisterByName(s string) *Register {
	for _, v := range p.Registers {
		if v.Name == s {
			return v
		}
	}
	return nil
}

func (p *Peripheral) FillRegisters() {
	var r []*Register
	var offset int64 // in units of bytes
	ridx := 0
	for _, v := range p.Registers {

		// if len(v.Fields) > 0 {
		// 	switch {
		// 	case v.Fields[0].BitOffset+v.Fields[0].BitWidth <= 8:
		// 		v.Size = 8
		// 	case v.Fields[0].BitOffset+v.Fields[0].BitWidth <= 16:
		// 		v.Size = 16
		// 	}
		// }

		// same address as previous: generate union field
		if len(r) > 1 && v.AddressOffset == r[len(r)-1].AddressOffset {
			r[len(r)-1].Union = append(r[len(r)-1].Union, v)
			continue
		}

		if v.AddressOffset%(v.Size/8) != 0 {
			log.Fatalf("Unaligned field %s:%s [%d] at %d bit", p.Name, v.Name, v.Size, v.AddressOffset)
		}

		gap := int64(v.AddressOffset) - offset

		if gap > 0 {
			r = append(r, &Register{Name: fmt.Sprintf("RESERVED%d", ridx), AddressOffset: number(offset), Size: 8, Access: "reserved", ArraySize: uint64(gap)})
			offset += gap
			ridx++
		}

		r = append(r, v)
		offset += int64(v.Size / 8)
	}
	p.Registers = r
}

// there's only one device of this type, (so we can omit the argument from the accessor functions)
func (p *Peripheral) IsSingleton() bool { return p.DerivedFrom == "" && len(p.Others) == 0 }

type Interrupt struct {
	Name        string `xml:"name"`
	Description string `xml:"description"`
	Value       int    `xml:"value"`
}

type Register struct {
	Name          string   `xml:"name"`
	DisplayName   string   `xml:"displayName"`
	Description   string   `xml:"description"`
	AddressOffset number   `xml:"addressOffset"`
	Size          number   `xml:"size"`
	Access        string   `xml:"access"`
	ResetValue    number   `xml:"resetValue"`
	Fields        []*Field `xml:"fields>field"`
	// these are syntesized by analyzing the device
	Union     []*Register `xml:-`
	ArraySize uint64      `xml:-`
}

func (r *Register) FieldByName(s string) *Field {
	for _, v := range r.Fields {
		if v.Name == s {
			return v
		}
	}
	return nil
}

func (r *Register) HasEnumFields() bool {
	for _, v := range r.Fields {
		if v.BitWidth < r.Size {
			return true
		}
	}
	return false
}

type Field struct {
	Name        string `xml:"name"`
	Description string `xml:"description"`
	BitOffset   number `xml:"bitOffset"`
	BitWidth    number `xml:"bitWidth"`
}

func (f *Field) String() string {
	if f.BitWidth > 1 {
		return fmt.Sprintf("[%d:%d]%s", f.BitOffset+f.BitWidth-1, f.BitOffset, f.Name)
	}
	return fmt.Sprintf("[%d]%s", f.BitOffset, f.Name)
}

type byName []*Peripheral

func (b byName) Len() int           { return len(b) }
func (b byName) Swap(i, j int)      { b[i], b[j] = b[j], b[i] }
func (b byName) Less(i, j int) bool { return b[i].Name < b[j].Name }

type byValue []*Interrupt

func (b byValue) Len() int           { return len(b) }
func (b byValue) Swap(i, j int)      { b[i], b[j] = b[j], b[i] }
func (b byValue) Less(i, j int) bool { return b[i].Value < b[j].Value }

type byAddressOffset []*Register

func (b byAddressOffset) Len() int           { return len(b) }
func (b byAddressOffset) Swap(i, j int)      { b[i], b[j] = b[j], b[i] }
func (b byAddressOffset) Less(i, j int) bool { return b[i].AddressOffset < b[j].AddressOffset }

type byBitOffset []*Field

func (b byBitOffset) Len() int           { return len(b) }
func (b byBitOffset) Swap(i, j int)      { b[i], b[j] = b[j], b[i] }
func (b byBitOffset) Less(i, j int) bool { return b[i].BitOffset > b[j].BitOffset }

type number uint64

func (hn *number) UnmarshalXML(d *xml.Decoder, start xml.StartElement) error {
	var s string
	err := d.DecodeElement(&s, &start)
	if err != nil {
		return err
	}
	n, err := strconv.ParseUint(s, 0, 64)
	*hn = number(n)
	return err
}

func (hn number) Hex() string { return fmt.Sprintf("0x%X", uint64(hn)) }

var tmplfuncs = template.FuncMap{
	"lower": strings.ToLower,
	"upper": strings.ToUpper,
}

func cleanws(s *string) { *s = strings.Join(strings.Fields(*s), " ") }

var (
	fDebug = flag.Bool("d", false, "dump parsed device struct")
	fTempl = flag.String("t", "", "name of template file")
	fList  = flag.String("i", "", "if not empty, only include devices named in this file")
)

func main() {

	log.SetFlags(0)
	log.SetPrefix("genstruct: ")
	flag.Parse()

	if len(flag.Args()) != 1 {
		log.Fatalf("Usage: %s [-d] [-i incl.lst] [-t path/to/lang.tmpl] path/to/stm32xxxx.svd", os.Args[0])
	}

	var (
		tmpl *template.Template
		err  error
		incl map[string]bool
	)

	if *fList != "" {
		buf, err := os.ReadFile(*fList)
		if err != nil {
			log.Fatalln(err)
		}
		incl = map[string]bool{}
		for _, v := range strings.Fields(string(buf)) {
			incl[v] = true
		}
	}

	if *fTempl == "" {
		tmpl = template.Must(template.New("list").Funcs(tmplfuncs).Parse(listTmpl))
	} else {
		tmpl, err = template.New(filepath.Base(*fTempl)).Funcs(tmplfuncs).ParseFiles(*fTempl)
		if err != nil {
			log.Fatal(err)
		}
		log.Println("template file:", tmpl.Name())
	}

	f, err := os.Open(flag.Arg(0))
	if err != nil {
		log.Fatalln(err)
	}
	dname, fname := filepath.Split(f.Name())
	basename := strings.ToLower(strings.TrimSuffix(fname, filepath.Ext(fname)))
	log.Println(dname, fname, basename)
	var device Device

	if err := xml.NewDecoder(f).Decode(&device); err != nil {
		log.Fatal(err)
	}
	f.Close()

	index := map[string]string{}

	if device.AddressUnitBits != 8 || device.Width != 32 {
		log.Fatal("can only work on assuming AddressUnitBits = 8, got %d and device.Width = 32, got %d", device.AddressUnitBits, device.Width)
	}

	if incl != nil {
		vv := device.Peripherals[:0]
		for _, v := range device.Peripherals {
			if incl[v.Name] {
				vv = append(vv, v)
			}
		}
		device.Peripherals = vv
	}

	for _, v := range device.Peripherals {
		for _, w := range v.Registers {
			if w.Size != 32 {
				log.Println("%s %s register has bad size %d", v.Name, w.Name, w.Size)
			}
		}
	}

	// Cleanup whitespace in description fields, sort
	sort.Sort(byName(device.Peripherals))
	for _, v := range device.Peripherals {
		cleanws(&v.Description)

		sort.Sort(byValue(v.Interrupts))
		for _, w := range v.Interrupts {
			cleanws(&w.Description)
		}
		device.Interrupts = append(device.Interrupts, v.Interrupts...)

		sort.Sort(byAddressOffset(v.Registers))

		devpfx := v.Name + "_"

		for _, w := range v.Registers {

			w.Name = strings.TrimPrefix(w.Name, devpfx)

			cleanws(&w.Description)

			if len(w.Fields) == 0 {
				continue
			}

			sort.Sort(byBitOffset(w.Fields))

			if len(w.Fields) == int(w.Fields[0].BitOffset+w.Fields[0].BitWidth) && len(w.Fields) > 1 {
				// only 1 bit fields, dense from lsb upward
				t := nameTemplate(w.Fields[0].Name)
				allsame := true
				for _, v := range w.Fields[1:] {
					if t != nameTemplate(v.Name) {
						allsame = false
						break
					}
				}
				if allsame {
					w.Fields[0].BitWidth = w.Fields[0].BitOffset + 1
					w.Fields[0].BitOffset = 0
					w.Fields[0].Name = t
					w.Fields[0].Description = "Merged " + w.Fields[0].Description
					w.Fields = w.Fields[:1]
				}
			}

			for _, x := range w.Fields {
				cleanws(&x.Description)
				index[v.Name+"::"+w.Name] += x.String()
			}
		}
	}

	if len(device.Interrupts) > 0 {
		sort.Sort(byValue(device.Interrupts))
		vv := device.Interrupts[:1]
		for _, v := range device.Interrupts[1:] {
			if v.Value != vv[len(vv)-1].Value {
				vv = append(vv, v)
			}
			if v.Name != vv[len(vv)-1].Name {
				log.Printf("Conflicting interrupt definitions %v and %v", v, vv[len(vv)-1])
			} else {
				//log.Printf("Duplicate interrupt definition %v", v)
			}
		}
		device.Interrupts = vv
	} else {
		log.Println("No interrupts defined.")
	}

	if *fDebug {
		enc := json.NewEncoder(os.Stdout)
		enc.SetIndent("", "\t")
		enc.Encode(&device)
		return
	}

	// revindex := map[string][]string{}
	// for k, v := range index {
	// 	revindex[v] = append(revindex[v], k)
	// }

	for k1, v1 := range device.Peripherals {
		for k2, v2 := range device.Peripherals {
			if k1 == k2 {
				continue
			}
			if v1.DerivedFrom != "" {
				continue
			}
			if v2.DerivedFrom != "" {
				continue
			}
			if isSuperset(v1, v2) {
				if isSuperset(v2, v1) {
					log.Printf("%s is identical to %s", v1.Name, v2.Name)
					v2.DerivedFrom = v1.Name
					v2.Registers = nil
				} else {
					log.Printf("%s is derived type of base type %s", v1.Name, v2.Name)
					v1.Extends = append(v1.Extends, v2)
				}
			}
		}
	}

	for _, v := range device.Peripherals {
		if vv := device.PeripheralType(v.Name); vv != v {
			v.DerivedFrom = vv.Name
			vv.Others = append(vv.Others, v)
			v.Registers = nil
		} else {
			v.FillRegisters()
		}
	}

	// fill the renameMap: for every device xxxx, if all xxxxNN are the same type, call the type xxxx
	namestemtotype := map[string]map[string]bool{}
	for _, v := range device.Peripherals {

		// GPIO[A..H] are the only ones that dont follow the stem.digit format
		// don't rename them
		if strings.HasPrefix(v.Name, "GPIO") {
			continue
		}

		n := nameStem(v.Name)
		t := device.PeripheralType(v.Name).Name
		//		log.Println("type", v.Name, "stem", n, "is", t)
		if namestemtotype[n] == nil {
			namestemtotype[n] = map[string]bool{t: true}
		} else {
			namestemtotype[n][t] = true
		}
	}
	for k, v := range namestemtotype {
		if len(v) == 1 {
			for kk, _ := range v {
				//				log.Println("rename", kk, "to", k)
				renameMap[kk] = k
			}
		}

	}

	if err := tmpl.Execute(os.Stdout, &device); err != nil {
		log.Fatal(err)
	}
}

// if the big has all the registers and all the bits of the small
func isSuperset(big, small *Peripheral) bool {
	for _, v := range small.Registers {
		vv := big.RegisterByName(v.Name)
		if vv == nil {
			return false
		}
		if vv.AddressOffset != v.AddressOffset {
			return false
		}
		if vv.Size != v.Size {
			return false
		}
		for _, fs := range v.Fields {
			fb := vv.FieldByName(fs.Name)
			if fb == nil {
				return false
			}
			if fb.BitOffset != fs.BitOffset {
				return false
			}
			if fb.BitWidth != fs.BitWidth {
				return false
			}
		}
	}
	return true
}

func nameTemplate(s string) string {
	return strings.Join(strings.FieldsFunc(s+" ", unicode.IsDigit), "X")
}

func nameStem(s string) string {
	if strings.HasPrefix(s, "I2C") { // hack
		return "I2C"
	}
	return strings.Join(strings.FieldsFunc(s, unicode.IsDigit), "")
}

var listTmpl = `
{{range .Peripherals}}
{{- if .DerivedFrom}}
{{- else}}
{{.Name}}{{range .Others}} {{.Name}}{{end}}
{{- end}}
{{- end}}
`
