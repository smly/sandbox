# Export workspace
mkdir -p $HOME/gitws
cd $HOME/gitws
git clone https://github.com/smly/sandbox
ln -s $HOME/gitws/sandbox/golang-sandbox $HOME/gocode
export GOPATH=$HOME/gocode
