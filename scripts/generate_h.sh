#!/bin/bash

# «Camelion» - Perl storable/C struct back-and-forth translator
#
#  Copyright (C) Alexey Shishkin 2016
#
#  This file is part of Project «Camelion».
#
#  Project «Camelion» is free software: you can redistribute it and/or modify
#  it under the terms of the GNU Lesser General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  Project «Camelion» is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
#  GNU Lesser General Public License for more details.
#
#  You should have received a copy of the GNU Lesser General Public License
#  along with Project «Camelion». If not, see <http://www.gnu.org/licenses/>.

TARGET="./libcamelion.h"

LICENSE="/* «Camelion» - Perl storable/C struct back-and-forth translator
*
*  Copyright (C) Alexey Shishkin 2016
*
*  This file is part of Project «Camelion».
*
*  Project «Camelion» is free software: you can redistribute it and/or modify
*  it under the terms of the GNU Lesser General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  Project «Camelion» is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
*  GNU Lesser General Public License for more details.
*
*  You should have received a copy of the GNU Lesser General Public License
*  along with Project «Camelion». If not, see <http://www.gnu.org/licenses/>.
*/"
GENERATOR="/* This file was automatically generated by generate_h.sh, `date +"%D %T"` */"
HDEFNAME="LIBCAMELION_H"

# Say
echo "# Started generate_h.sh"
#cd ../../camelion

# Remove old file
rm $TARGET

# Find header-files
HFILES=$(find -name "*.h") # | sed 's/\.\///')

Headers()
{
    cat "$1"           |
    grep '^#include "' |
    tr -d '"'          |
    awk '{print $2}'   | #sed 's/\.\///'
    sed 's/\.\./\./'
}

HLIST=""

Scan()
{
    HLIST=$(echo -e "$1\n$HLIST")
    local HRS=$(Headers "$1")
    local f
    for f in $HRS;
    do
        Scan "$f"
    done
}

for i in $HFILES;
do
    Scan "$i"
done

# Add license text
echo "$LICENSE" > $TARGET
# Add generator text
echo "$GENERATOR" >> $TARGET
# Add ifndef statement
echo "#ifndef $HDEFNAME" >> $TARGET
echo "#define $HDEFNAME" >> $TARGET
echo >> $TARGET

# Includes @todo need something more smarty
echo "#include <stdint.h>" >> $TARGET
# Remove duplicates from includes list
HLIST=$(echo "$HLIST" | awk '!a[$0]++')

# Cycle for files
for i in $HLIST;
do
    # Insert file name
    echo -e "\n/* Exported from $i */" >> $TARGET
    # Scan for exportable define statements
    awk '
/\/\*\ E\-\ \*\// {
  set = 0;
  printf("\n");
}
{ if (set) print $0; }
/\/\*\ E\+\ \*\// {
  set=1;
}' $i >> $TARGET
    # Scan for functions, structs and enums
    awk '
/\/\* E \*\// {
  sub(/\/\* E \*\/ /,"",$0);
  level=1
  count=0
}
level {
  n = split($0, c, "");
  for (i = 1; i <= n; i++)
  {
    printf(c[i]);
    if (c[i] == ";")
    {
        if(level==1)
        {
            level = 0;
            if (count != 0)
                printf("\n");
        };
    }
    else if (c[i] == "{")
    {
        level++;
        count++;
    }
    else if (c[i] == "}")
    {
        level--;
        count++;
    }
  }
  printf("\n")
}' $i >> $TARGET
done

## Finish file
echo >> $TARGET
echo "#endif //$HDEFNAME" >> $TARGET

# Say
echo "# Finished generate_h.sh"
