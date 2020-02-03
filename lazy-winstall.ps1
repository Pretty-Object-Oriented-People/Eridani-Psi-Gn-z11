##############
# Make Tools #
##############
New-Item -ItemType Directory -Force -Path tools/win
cd ./tools/win
function DownloadUnzip($url, $folder){
	wget "$url" -outfile TMP.zip
	Expand-Archive "tmp.zip" "$folder"
	rm "tmp.zip"
}
function DownloadUntar($url){
	wget "$url" -outfile TMP.tar.gz
	tar -xf "TMP.tar.gz"
	rm "TMP.tar.gz"
}
# GNU Core
if(-Not (Test-Path "coreutils/manifest/coreutils-*-bin.mft")){
	echo "Installing CoreUtils..."
	DownloadUnzip 'https://netix.dl.sourceforge.net/project/gnuwin32/coreutils/5.3.0/coreutils-5.3.0-bin.zip' "coreutils"
	echo "OK."
} else { echo "CoreUtils already installed" }
if(-Not (Test-Path "coreutils/manifest/coreutils-*-dep.mft")){
	echo "Installing CoreUtils deps..."
	DownloadUnzip 'https://netix.dl.sourceforge.net/project/gnuwin32/coreutils/5.3.0/coreutils-5.3.0-dep.zip' "coreutils"
	echo "OK."
} else { echo "CoreUtils deps already installed" }
# GNU Sed
if(-Not (Test-Path "sed/manifest/sed-*-dep.mft")){
	echo "Installing SED..."
	DownloadUnzip 'https://netix.dl.sourceforge.net/project/gnuwin32/sed/4.2.1/sed-4.2.1-bin.zip' "sed"
	echo "OK."
} else { echo "SED already installed" }
if(-Not (Test-Path "sed/manifest/sed-*-dep.mft")){
	echo "Installing SED deps..."
	DownloadUnzip 'https://netix.dl.sourceforge.net/project/gnuwin32/sed/4.2.1/sed-4.2.1-dep.zip' "sed"
	echo "OK."
} else { echo "SED deps already installed" }
#
cd ../..

#############
# Libraries #
#############
New-Item -ItemType Directory -Force -Path lib/Windows
cd lib/Windows
#JSON-C
if(-Not (Test-Path "libjson-c.dll.a")){
	copy "json-c/libjson-c.dll.a" "libjson-c.dll.a"
}
#
cd ../..

########
# DLLs #
########
cd run
#JSON-C
if(-Not (Test-Path "libjson-c.dll")){
	copy "../lib/Windows/json-c/libjson-c.dll" "libjson-c.dll"
}
#
cd ..

##############
# Check Make #
##############
make printinfo