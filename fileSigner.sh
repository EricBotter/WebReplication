#!/bin/sh

webpath="/var/webr/websites"
sigpath="/var/webr/signatures"
keypath="/var/webr/keys"

for i in `ls -d $webpath/*`; do
	current=`basename $i`
	key=$keypath/$current/private.pem
	if [ ! -f $key ]; then
		echo "Unable to sign website $current, private key not found."
		echo " - Please place an unencrypted private key in folder $keypath/$current"
		continue
	fi
	echo "Signing website $current..."

	for file in `find $webpath/$current -type f`; do
		directory=`dirname $sigpath${file:${#webpath}}.sig`
		#echo $key hhh $sigpath${file:${#webpath}}.sig hhh $file
		if [ ! -d $directory ]; then
			mkdir $directory
		fi
	openssl dgst -sha256 -sign $key -out $sigpath${file:${#webpath}}.sig $file
	done
done

echo "Done."
