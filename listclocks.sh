#!/bin/bash

SERVICE="_http._tcp"

dns-sd -Z "$SERVICE" local 2>/dev/null | awk '
/PTR/ && /(espclock-|eyedro-)/ {
    name=$NF
    sub(/\._http._tcp$/, "", name)

    type="unknown"
    version="unknown"
    fname="unknown"

    # read until we hit TXT line
    while (getline) {
        if ($0 ~ /TXT/) {
            # extract quoted fields
            while (match($0, /"[^"]+"/)) {
                field = substr($0, RSTART+1, RLENGTH-2)
                sub(/^[^=]+=/, "", field_value)
                
                split(field, kv, "=")

                if (kv[1] == "type") type = kv[2]
                if (kv[1] == "version") version = kv[2]
                if (kv[1] == "name") fname = kv[2]

                $0 = substr($0, RSTART + RLENGTH)
            }

            print "---- " name " ----"
            print "Type:    " type
            print "Version: " version
            print "Name:    " fname
	    print "http://"name".local/"
            print ""
            break
        }
    }
}
'
