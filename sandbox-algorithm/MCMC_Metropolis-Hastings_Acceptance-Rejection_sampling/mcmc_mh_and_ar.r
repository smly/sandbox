my.rgamma <- function()
{
  k <- 2.0
  while (1){
    x <- rcauchy(1, 0.4 ,0.6)
    alpha <- dgamma(x, 2.0, 2.5) / (k * dcauchy(x, 0.4, 0.6))
    if(runif(1) < alpha){
      return(x)
    }
  }
}

AR.rgamma <- function (n)
{
  v <- rep(NA, n)
  for (i in 1:n) {
    v[i] <- my.rgamma()
  }
  v
}

MH.rgamma <- function (n)
{
  mu <- 0.8
  sig <- 3.06

  v <- rep(NA, n)
  x <- 0.8
  v[1] <- x
  for (i in 2:n) {
    x.prime<- rnorm(1, mu, sig)
    prob <- min(1,
                (dgamma(x.prime, a, b) / dgamma(x,a,b)) *
                (dnorm(x, mu, sig) / dnorm(x.prime, mu, sig))
                )
    if (runif(1) < prob)
      x <- x.prime
    v[i] <- x
  }
  v
}

r1.AR.rgamma <- AR.rgamma(10000)
r1.MH.rgamma <- MH.rgamma(10000)
r1.rgamma <- rgamma(10000, 2.0, 2.5)
r1.linear <- c( -500:500 ) / 100
postscript("armh4.ps", width="500", height="500")
par(new=FALSE)
plot(r1.linear, r1.linear, type="l",
     xlim=c(0,5), ylim=c(0,5), pch=1, col="blue")
par(new=TRUE)
qqplot(r1.MH.rgamma, r1.rgamma, ylab="", xlab="",
       xlim=c(0,5), ylim=c(0,5), pch=4, col="green")
par(new=TRUE)
qqplot(r1.AR.rgamma, r1.rgamma, ylab="", xlab="",
       xlim=c(0,5), ylim=c(0,5), pch=4, col="red")
legend(3.5, 1.5, c("A-R", "M-H", "Linear"),
       lwd=3, col=c("red","green","blue"))
dev.off()
#vec <- AR.rgamma(10000, 2.3, 2.7)
#vec <- MH.rgamma(10000, 2.3, 2.7)
#vec <- rgamma(10000, 2, 2.5)
#par(mfrow=c(2,1))
#plot(ts(vec))
#hist(vec,30)
#par(mfrow=c(1,1))

## x <- c( -1000:1000 ) / 300
## y <- dcauchy(x, 0.4, 0.6)
## y2 <- dgamma(x, 2, 2.5)
## par(new=FALSE)
## plot(x, y, type="l")
## par(new=TRUE)
## plot(x, y2, type="l")
