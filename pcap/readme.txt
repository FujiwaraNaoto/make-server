
sudo apt-get install libpcap-dev
で pcapライブラリをインストールする.
linuxで動く.
gcc -Wall ether_hdr.c -o 実行ファイル名 -lpcap でコンパイルされる.
管理者権限下で実行
sudo ./管理者権限
