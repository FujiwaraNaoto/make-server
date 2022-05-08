以下のコマンド
```
sudo apt-get install libpcap-dev
```
で pcapライブラリをインストールする.
linuxで動く.
コンパイルの際には
```
gcc -Wall ether_hdr.c -o 実行ファイル名 -lpcap
```
 でコンパイルされる.
管理者権限下で実行する必要があり
```
sudo ./実行ファイル名
```