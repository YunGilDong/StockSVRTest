#!/bin/sh

KILO=0
MEGA=0
GIGA=0
SUM=0

echo -e "---------------------------------------"
echo -e "         DISK USAGE PER USER"
echo -e "---------------------------------------"

echo "userid            homedir        used"
echo -e "---------------------------------------"

i=1

user=`cat /etc/passwd | grep /bin/bash | cut -f 1 -d: | tail -n $i | head -n 1`

while [ : ];
do
	if [ "$user" != "root" ]; then 
		dir=`cd /home/$user;pwd`
		dush=`du -sh /home/$user |cut -f 1`
		KILO=`du -s /home/$user | cut -f 1`
		SUM=$(expr $SUM + $KILO)
	else
		dir=`cd /root;pwd`
		dush=`du -sh /root |cut -f 1`
		KILO=`du -s /root | cut -f 1`
		printf "%s \t\t %s \t\t %s \t\t\n" "$user" "$dir" "$dush" 
		SUM=$(expr $SUM + $KILO)
		break
	fi

	i=$(expr $i + 1)
	#SUM=$(expr $SUM + $KILO)	
	printf "%s \t\t %s \t %s \t\n" "$user" "$dir" "$dush" 
	user=`cat /etc/passwd | grep /bin/bash | cut -f 1 -d: | tail -n $i | head -n 1`

done

KILO=$SUM
MEGA=`echo "$SUM 1024"|awk '{printf "%.2f", $1 / $2}'` 
GIGA=`echo "$SUM 1024"|awk '{printf "%.3f", $1 / ($2*$2)}'`

echo -e "---------------------------------------"
echo -e "Total : $KILO K    $MEGA M       $GIGA G"
echo -e "---------------------------------------"
