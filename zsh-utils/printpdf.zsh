function say() {
  curl -d "message=$1" http://im.kayac.com/api/post/smly
}

# TATE MONOTONE
function printpdf() {
  scp $1 kohei-o@rayserv61:~/.tmp.pdf
  ssh kohei-o@rayserv61 "acroread -toPostScript -shrink < ~/.tmp.pdf | ./duplex-lpr -long | lpr -Pals7p" && echo "done."
}

function printpdf_color() {
  scp $1 kohei-o@rayserv61:~/.tmp.pdf
  ssh kohei-o@rayserv61 "acroread -toPostScript -shrink < ~/.tmp.pdf | ./duplex-lpr -long | lpr -Palspr" && echo "done."
}

# YOKO COLOR
function printpdf_yoko() {
  scp $1 kohei-o@rayserv61:~/.tmp.pdf
  ssh kohei-o@rayserv61 "acroread -toPostScript -landscape < ~/.tmp.pdf | ./duplex-lpr -short | lpr -Palspr" && echo "done."
}

function printpdf_hoge() {
  ssh kohei-o@rayserv61 "echo hi"
}

# RONBUN COLOR
function printpdf_wget() {
  ssh kohei-o@rayserv61 "wget --no-check-certificate --user-agent=\"Mozilla/5.0 (X11; U; Linux i686; pl-PL; rv:1.9.0.2) Gecko/20121223 Ubuntu/9.25 (jaunty) Firefox/3.8\" \"$1\" -O ~/.tmp.pdf && acroread -toPostScript < ~/.tmp.pdf | ./duplex-lpr -long | lpr -Palspr" && echo "done."
}
