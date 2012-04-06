#!/bin/sh

INCLUDE_DIRS="-I /usr/include/soem -I /usr/local/include/soem -I ../src"

EC_TYPE="ethercattype.h"
EC_BASE="${EC_TYPE} ethercatbase.h"
EC_COE="${EC_TYPE} ethercatmain.h ethercatcoe.h"
EC_CONFIG="${EC_TYPE} ethercatconfig.h"

# It seems that "ethercatconfiglist.h" isn't used at all, and contains undefined type
#EC_CONFIGLIST="${EC_TYPE} ethercatconfiglist.h"

EC_DC="${EC_TYPE} ethercatdc.h"
EC_FOE="${EC_TYPE} ethercatfoe.h"
EC_MAIN="${EC_TYPE} ethercatmain.h"
EC_PRINT="${EC_TYPE} ethercatprint.h"
EC_SOE="${EC_TYPE} ethercatsoe.h"
EC_NICDRV="${EC_TYPE} nicdrv.h"

h2xml $INCLUDE_DIRS $EC_TYPE -o ethercattype.xml
h2xml $INCLUDE_DIRS $EC_BASE -o ethercatbase.xml
h2xml $INCLUDE_DIRS $EC_COE -o ethercatcoe.xml
h2xml $INCLUDE_DIRS $EC_CONFIG -o ethercatconfig.xml
# h2xml $INCLUDE_DIRS $EC_CONFIGLIST -o ethercatconfiglist.xml
h2xml $INCLUDE_DIRS $EC_DC -o ethercatdc.xml
h2xml $INCLUDE_DIRS $EC_FOE -o ethercatfoe.xml
h2xml $INCLUDE_DIRS $EC_MAIN -o ethercatmain.xml
h2xml $INCLUDE_DIRS $EC_PRINT -o ethercatprint.xml
h2xml $INCLUDE_DIRS $EC_SOE -o ethercatsoe.xml
h2xml $INCLUDE_DIRS $EC_NICDRV -o nicdrv.xml

#find /usr/local/include/soem/ -iname "*.h" -printf "h2xml $INCLUDE_DIRS ethercattype.h %P -o %P\n" | sed "s/\.h/\.xml/3" |sh

find . -iname "*.xml" -printf "xml2py -l soem %P -o %P\n" | sed "s/\.xml/\.py/2" |sh
