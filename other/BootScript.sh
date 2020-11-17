#!/bin/bash

# Compare two versions:
# Return 0 - $1 == $2
#        1 - $1 > $2
#        2 - $1 < $2
vercomp () {
    if [[ $1 == $2 ]]
    then
        return 0
    fi
    local IFS=.
    local i ver1=($1) ver2=($2)
    # fill empty fields in ver1 with zeros
    for ((i=${#ver1[@]}; i<${#ver2[@]}; i++))
    do
        ver1[i]=0
    done
    for ((i=0; i<${#ver1[@]}; i++))
    do
        if [[ -z ${ver2[i]} ]]
        then
            # fill empty fields in ver2 with zeros
            ver2[i]=0
        fi
        if ((10#${ver1[i]} > 10#${ver2[i]}))
        then
            return 1
        fi
        if ((10#${ver1[i]} < 10#${ver2[i]}))
        then
            return 2
        fi
    done
    return 0
}


mkdir -p /var/atlasdata

if [ ! -d /nvram/atlas ]; then
    cp -r /usr/local/atlas/var /nvram/atlas
else
    ver_usr=$(cat /usr/local/atlas/var/comp_version.txt)
    ver_nvram=$(cat /nvram/atlas/comp_version.txt)

    vercomp $ver_usr $ver_nvram

    if [ $? -eq 1 ]; then
        rm -rf /nvram/atlas/atlas-probe/bin
        rm -rf /nvram/atlas/atlas-probe/crons
        rm -rf /nvram/atlas/atlas-probe/data
        rm -rf /nvram/atlas/atlas-probe/run
        rm -rf /nvram/atlas/atlas-probe/status
        rm -rf /nvram/atlas/atlas-probe/state

        cp -r /usr/local/atlas/var/* /nvram/atlas
    fi
fi

if [ ! -L /var/atlas-probe ]; then
        ln -s /nvram/atlas/atlas-probe /var/atlas-probe
        # ln -s /nvram/atlas/atlasdata /var/atlasdata
        ln -s /var/atlasdata /var/atlas-probe/data
fi
