#!/bin/bash
rm -rf release
mkdir -p release

cp -rf Jk *.{hpp,cpp,txt,json} LICENSE release/

mv release score-addon-jk
7z a score-addon-jk.zip score-addon-jk
