
まず main.cの
const char WEBROOT[] = "/Users/fujiwara/desktop/make-practice/makeserver/tinyweblog2/";//
const char LOGFILE[] = "/Users/fujiwara/desktop/make-practice/makeserver/tinyweblog2/tinywebd.log";
の部分をindex.htmlが置いてあるパスに書き換えましょう.

次に
Makefile を実行します　これはコンパイルを行うためのものです.
ターミナルで
make
と打ちましょう.
すると
gcc -c DieWithError.c
gcc -c hacking.c
gcc -c hacking-network.c
gcc -c main.c
gcc -c logger.c
gcc DieWithError.o hacking.o hacking-network.o main.o logger.o -Wall -o main

となります.
続いて　実行ファイルに権限を持たせます. 
これは実行ファイルが　port番号が80番のwell-known portを開けようとしているからです.
ターミナルで次を打ちましょう
sudo chown root:root ./main
sudo chmod u+s ./main

これで管理者権限で実行できます.
その前にラズパイではデフォルトで 80番portにはapacheが動くので
sudo systemctl stop apache2
sudo systemctl disable apache2

としておきましょう.

続いて mainを実行しましょう
./main

このまま web browser を開いて
http://ラズパイのipアドレス　としましょう

もしくはラズパイのブラウザ上で http://127.0.0.1と打ち込みましょう




ターミナルで こんな感じのメッセージが表示されたら成功
in recv_line
GET / HTTP/1.1
Accept a request from 192.168.43.133. port number = 50722. request = GET / HTTP/1.1
in resource /index.html' 	200 OK
in recv_line
GET /xmas_wallpaper.png HTTP/1.1
Accept a request from 192.168.43.133. port number = 50725. request = GET /xmas_wallpaper.png HTTP/1.1
in resource /xmas_wallpaper.png	 Open '/xmas_wallpaper.png' 	200 OK
in recv_line

プログラムを止めるには Ctrl + C と打ち込みましょう

ログファイルを見てみましょう.
sudo cat tinywebd.log
で表示されるはずです



