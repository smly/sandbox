# sandbox-os

OS 依存のコード、システムコールを扱うプログラム、コンパイラのバージョン依存の問題などのメモ書きでう。移植性については、いつも使う環境がだいたい Linux すこしだけ Darwin なので、いくらか Linux 贔屓な点があります。**メモ書きなので内容に期待してはだめです**。

だいたい移植性の高いコードを書く・異なる OS 上で動作することを想定したコードを読む・古いコードを読むこと考えて忘れそうなことをメモしている。どの仕様で deprecated か、どのバージョンで問題が起きるのかを可能であれば（暇であれば）調べる。

短い話や未調査の話題はリスト形式で、コードや引用を含む長い話は小節で記述する。

* サポートする version は `SYSCALLS(2)` があるからここに書かなくてよかった．http://www.kernel.org/doc/man-pages/online/pages/man2/syscalls.2.html

## sync_file_range()

* マクロを使って glibc のバージョンで関数があるかないか判定することがよくあること
* RHEL 5 の sync_file_range() は未実装．バグ．https://bugzilla.redhat.com/show_bug.cgi?id=518581
** redis 2.6.0 → 2.6.1 差分で知った https://github.com/antirez/redis/pull/730 https://github.com/antirez/redis/issues/667
** 2012-01-31 に RHEL 5 のサポートが延長されて 2017-03-31 までとなっている http://www.nec.co.jp/linux/linux-os/rhel10y.html

## sendfile()

* linux では sys/sendfile.h
* read(2) して write(2)/send(2) する。カーネルバッファが用いられるので扱いやすい上に効率的
* Darwin の場合は sendfile が別の header にあり、かつ return value の扱いが異なる＆シグニチャが異なるのでエミュレートしたりする
  * たとえば samba は sys_sendfile を定義して OS による違いを吸収している
  * ref. <http://gitweb.samba.org/?p=samba.git;a=blob;f=source3/lib/sendfile.c>
* 懸念, 例えば Apache のドキュメントなど:
  * ref. "The default is now off because kernel sendfile() is not always able to work 
on all file objects." <https://groups.google.com/group/modwsgi/browse_thread/thread/328321ef736ac5fc?hl=ja>
  * ref. "But on some platforms or within some filesystems, it is better to disable this feature to avoid operational problems" <http://httpd.apache.org/docs/2.2/mod/core.html#enablesendfile>
  * ref. sendfile() syscall and why the 2.6 linux kernel sucks! <http://ilia.ws/archives/13-sendfile-syscall-and-why-the-2.6-linux-kernel-sucks!.html>

## eventfd

* Linux `2.6.22` 以降.
* Linux `2.6.27` 以降で EFD_NONBOCK など flag 指定化（それ以前は 0 を常に渡す）
  * <http://codezine.jp/article/detail/4837> の説明にある EFD_NONBLOCK の動作は誤り。EFD_NONBLOCK のフラグが指定されている場合は `read() はエラー -1 を返して errno に EAGAIN を設定する` が正しい。と思ったのだが「EAGAINを返して」の主語が「カーネル」なら正しいかもしれない。日本語は難しい。

以下、検証 (eventfd_nonblock.c):

<pre>
int main(int argc, char** argv) {
  // int efd = eventfd(0, 0);
  int efd = eventfd(0, EFD_NONBLOCK);
  uint64_t val = 0;
  ssize_t st;

  switch( fork() ) {
    case 0: // child
      sleep(2);

      val++;
      write(efd, &val, sizeof(uint64_t));
      printf("child write(): eventfd: %llu\n", val);

      close(efd);
      exit(EXIT_SUCCESS);

    default: // parent
      while (1) {
        printf("parent before read(): eventfd: %llu\n", val);

        st = read(efd, &val, sizeof(uint64_t));
        printf("parent after read(): eventfd: %llu\n", val);

        if (st == -1 && errno == EAGAIN) {
          sleep(1);
          continue;
        }

        close(efd);
        exit(EXIT_SUCCESS);
      }
  }
}
</pre>

結果：

<pre>
$ ./a.out
parent before read(): eventfd: 0
parent after read(): eventfd: 0
parent before read(): eventfd: 0
parent after read(): eventfd: 0
child write(): eventfd: 1
parent before read(): eventfd: 0
parent after read(): eventfd: 1
</pre>

## signalfd

* あとで

## timerfd

* Linux `2.6.25` 以降.
* あとで

## inotify

* linux kernel `2.6.13` から組み込まれる
* $ while ~/local/bin/inotifywait -e modify src/\*\*/\*.{cc,hh}; do make check; done とかして継続テストじゃーと遊んだりするときに便利
  * ただし src/\*\*/*.{cc,hh} は shell 展開なので実用的にはもう少し工夫が必要

## libev
  
* epoll/kqueue/event 同様 ports/inotify/eventfd/signalfd をサポート
  * signalfd, timerfd を ev_io, ev_timer などに置き換えて event loop に入れる感じ
* 内部的に使われる epoll は linux kernel へ `2.5.44` に初導入
* あとで

## gethostbyname(), gethostbyaddr()

* POSIX 2001 revision で deprecated になった.
* IPv6 に適合しない (ref. <http://beej.us/guide/bgnet/output/html/multipage/gethostbynameman.html>)
* 代わりに getaddrinfo(), getnameinfo()

## predefiend macro と include guard

* 最近では include guard として #pragma once を使っても問題ないと割り切れるようになった。
* \#pragma once を使わないで（当時は問題なかったが後になって）バグが発生したケースとして以下のように include gurad をしていたケースがある。

以下は Xsystem35 (ref. <http://8ne.sakura.ne.jp:20008/chika/unitbase/xsys35/>) のコードを grep したもの:

<pre>
src/counter.h:#ifndef __COUNTER__
</pre>

\__COUNTER__ は predefined macro として gcc 4.3 に追加されたためにこのコードは動作しなくなった。多くのコードは include guard に \__COUNTER_H__ のような命名規則を使っているので問題ないことが多いがこのような例外もある。pragma once directive の登場は以下, wikipedia (ref. <http://en.wikipedia.org/wiki/Pragma_once>) より:

<pre>
Compilers with #pragma once-related bugs included LCC-Win32 as of 2004 [5][6] and GCC as of 1998.[7] GCC originally gave a warning declaring #pragma once "obsolete" when compiling code that used it. However, with the 3.4 release of GCC, the #pragma once handling code was fixed to behave correctly with symbolic and hard links. The feature was "un-deprecated" and the warning removed.[8][9]
</pre>

* ほとんどのシステムは gcc 3.4 以降であると期待するなら使用しても問題ないと割り切れる。

## glibc getline()

* POSIX 2008.
* getline is defined in stdio.h in `glibc 2.10` and later
* CentOS4/RHEL3 は __glibc 2.3.4__, CentOS5/RHEL4 は __glibc 2.5__
* BSD libc fgetln() で glibc getline() を emulate する. あるいは fgets()
* BSD libc の getline() と glibc の getline() の違いは？ (TODO)

例えば OpenWrt は以下のコードでエミュレートしている:

* <https://dev.openwrt.org/browser/trunk/tools/include/getline.h>

<pre>
static inline ssize_t
getline(char **outbuf, size_t *outsize, FILE *fp)
{
	size_t len;

#ifndef __CYGWIN__
	char *buf;
	buf = fgetln(fp, &len);
#else
	char buf[512];
	fgets(buf, sizeof(buf), fp);	
	len = strlen(buf);
#endif
	if (buf == NULL)
		return (-1);

	/* Assumes realloc() accepts NULL for ptr (C99) */
	if (*outbuf == NULL || *outsize < len + 1) {
		void *tmp = realloc(*outbuf, len + 1);
		if (tmp == NULL)
			return (-1);
		*outbuf = tmp;
		*outsize = len + 1;
	}
	memcpy(*outbuf, buf, len);
	(*outbuf)[len] = '\0';
	return (len);
}
#endif
</pre>

## 他

* signalhandler
  * Linux 以外よくわからない。そもそも普段使わないので機会があれば調べる。
  * catch_signal.c
* TCP_CORK
  * Nagle algorithm (ref. <http://d.hatena.ne.jp/nyaxt/20090107>)
* todo: mmap, flock

## reference

* <http://codezine.jp/article/detail/4836>
* <http://www.oreilly.co.jp/community/blog/2010/03/pthread-epoll-inet-server-part1.html>
* <http://www.oreilly.co.jp/community/blog/2010/08/buffer-cache-and-aio-part1.html>
