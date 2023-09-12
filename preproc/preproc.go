package main

import (
	"bufio"
	"flag"
	"fmt"
	"log"
	"os"
	"path/filepath"
	"regexp"
	"sort"
	"strings"
)

var (
	reInc          = regexp.MustCompile(`^\s*#\s*include\s*"([^"]+)"`)        // #include "..."
	rePPD          = regexp.MustCompile(`^\s*#\s*([ie][fln]\S*)(\s+(\S.*))?`) // #include #if #elif #else and #endif
	reDef          = regexp.MustCompile(`^\s*#\s*define\s+(\S+)`)             // #define ...
	reStripComment = regexp.MustCompile(`(/\*.*\*/)|//.*$`)
)

func main() {

	flag.Parse()

	var fname, exprfname string

	switch len(flag.Args()) {
	case 2:
		exprfname = flag.Arg(1)
		fallthrough
	case 1:
		fname = flag.Arg(0)
	default:
		log.Fatalf("usage: %s header.h [exprs.txt]", os.Args[0])
	}

	exprs := map[string]bool{}
	if exprfname != "" {
		forEachLine(exprfname, func(fname string, lno int, s string) {
			exprs[s] = true
		})
	}

	if err := os.Chdir(filepath.Dir(fname)); err != nil {
		log.Fatal(err)
	}

	fname = filepath.Base(fname)

	if exprfname == "" {

		defines := map[string]bool{}

		forEachLine(fname, func(fname string, lno int, s string) {
			m := rePPD.FindStringSubmatch(s)
			if len(m) == 4 {
				v := strings.TrimSpace(reStripComment.ReplaceAllString(m[3], ""))

				switch m[1] {
				case "if", "elif":
					exprs[v] = true
				case "ifdef":
					exprs["defined ("+v+")"] = true
				case "ifndef":
					exprs["!defined ("+v+")"] = true
				}
			}
			m = reDef.FindStringSubmatch(s)
			if len(m) > 0 {
				v := strings.TrimSpace(reStripComment.ReplaceAllString(m[1], ""))
				defines[v] = true
				log.Printf("%s:%d %q\n", fname, lno, m)
			}
		})

		// dont include conditions for symbols defined in the source itself
		for k, _ := range defines {
			delete(exprs, "defined ("+k+")")
			delete(exprs, "!defined ("+k+")")
		}

		var ss []string
		for k, _ := range exprs {
			ss = append(ss, k)
		}
		sort.Strings(ss)
		for _, v := range ss {
			fmt.Println(v)
		}

	} else {

		state := []bool{true}
		statehist := []bool{true}

		forEachLine(fname, func(fname string, lno int, s string) {
			m := rePPD.FindStringSubmatch(s)
			if len(m) > 0 {
				log.Printf("%s:%d %q\n", fname, lno, m)

				v := ""
				if len(m) == 4 {
					v = strings.TrimSpace(reStripComment.ReplaceAllString(m[3], ""))
				}

				switch m[1] {
				case "if":
					state = append(state, exprs[v])
					statehist = append(statehist, state[len(state)-1])
				case "ifdef":
					state = append(state, exprs["defined ("+v+")"])
					statehist = append(statehist, state[len(state)-1])
				case "ifndef":
					state = append(state, exprs["!defined ("+v+")"])
					statehist = append(statehist, state[len(state)-1])
				case "elif":
					if len(state) > 0 {
						state[len(state)-1] = exprs[v] && !statehist[len(statehist)-1]
						statehist[len(statehist)-1] = statehist[len(statehist)-1] || state[len(state)-1]
					} else {
						log.Printf("%s:%d unexpected elif\n", fname, lno)
					}
				case "else":
					if len(state) > 0 {
						state[len(state)-1] = !state[len(state)-1] && !statehist[len(statehist)-1]
					} else {
						log.Printf("%s:%d unexpected else\n", fname, lno)
					}
				case "endif":
					if len(state) > 0 {
						state = state[:len(state)-1]
						statehist = statehist[:len(statehist)-1]
					} else {
						log.Printf("%s:%d unexpected endif\n", fname, lno)
					}
				}

				return
			}
			if state[len(state)-1] {

				m = reDef.FindStringSubmatch(s)
				if len(m) > 0 {
					v := strings.TrimSpace(reStripComment.ReplaceAllString(m[1], ""))
					exprs["!defined ("+v+")"] = true
					delete(exprs, "!defined ("+v+")")
				}

				fmt.Println(s)
			}
		})

	}
}

func forEachLine(fname string, fc func(fname string, lno int, s string)) {
	f, err := os.Open(fname)
	if err != nil {
		log.Fatal(err)
	}
	defer f.Close()
	log.Println("Processing", fname, "...")
	s := bufio.NewScanner(f)
	line := ""
	for lno := 1; s.Scan(); lno++ {
		if len(s.Text()) == 0 {
			continue
		}
		line += s.Text()
		if line[len(line)-1] == '\\' {
			line = line[:len(line)-1]
			continue
		}
		incstm := reInc.FindStringSubmatch(line)
		if len(incstm) == 2 {
			forEachLine(incstm[1], fc)
		} else {
			fc(fname, lno, line)
		}
		line = ""
	}
	if err := s.Err(); err != nil {
		log.Fatal(err)
	}
}
