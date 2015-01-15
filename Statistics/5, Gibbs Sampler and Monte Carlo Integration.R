#Problem 1.3

prob = matrix(c(1,0,0,0), 2, 2)
dual = matrix(c(.1,.3,.4, .2), 2, 2)
x_cond_y = matrix(c(.25, .75, 2/3, 1/3), 2, 2)
y_cond_x = matrix(c(.2, .6, .8, .4), 2, 2)
x_prob = c(.5,.5)
y_prob = c(.4,.6)

gibbs = function(m, prob, x_cond_y, y_cond_x){
  total_x_y = array(dim=c(2,2,m))
  total_y_x = array(dim=c(2,2,m))
  total_x_y[,1,1] = x_cond_y[,1] * colSums(prob)[1]
  total_x_y[,2,1] = x_cond_y[,2] * colSums(prob)[2]
  total_y_x[1,,1] = y_cond_x[1,] * rowSums(total_x_y[,,1])[1]
  total_y_x[2,,1] = y_cond_x[2,] * rowSums(total_x_y[,,1])[2]
  if(m > 1){
  for(l in 2:m){
    for(i in 1:2){
      total_x_y[,i,l] = x_cond_y[,i] * colSums(total_y_x[,,l - 1])[i]
    }
    for(i in 1:2){
      total_y_x[i,,l] = y_cond_x[i,] * rowSums(total_x_y[,,l])[i]
    }
  }
  }
  z = list(total_x_y, total_y_x)
  return(z)
}

gibbs(5,prob,x_cond_y, y_cond_x) #first array is x_t|y_t-1, second is y_t|x_t

#Problem 1.4
gibbs_chain = function(x,m,n_chains,x_cond_y, y_cond_x, disp = FALSE){
  total = array(dim=c(m,2,n_chains))
  a = matrix(rep(0,4), 2, 2)
  #total[1,1,1] = sample(1:2, 1, prob = x_cond_y[,x[2]])
  #total[1,2,1] = sample(1:2, 1, prob = y_cond_x[,x[1]])
  for(r in 1:n_chains){
    for(l in 1:m){
      if(l == 1){
        total[1,1,r] = sample(1:2, 1, prob = x_cond_y[,x[2]])
        total[1,2,r] = sample(1:2, 1, prob = y_cond_x[total[1,1,r],])
      }
      else{
        total[l,1,r] = sample(1:2, 1, prob = x_cond_y[,total[l - 1, 2, r]])
        total[l,2,r] = sample(1:2, 1, prob = y_cond_x[total[l, 1, r],])
      }
    }
    if(total[m,1,r] == 1){
      if(total[m,2,r] == 1) a[1,1] = a[1,1] + 1
      else a[1,2] = a[1,2] + 1
    }
    else{
      if(total[m,2,r] == 1) a[2,1] = a[2,1] + 1
      else a[2,2] = a[2,2] + 1
    }
  }
  prop = matrix(c(a[1,1]/n_chains,a[1,2]/n_chains,a[2,1]/n_chains,a[2,2]/n_chains),2,2,byrow = TRUE)
  if(disp) z = list(total,a,prop)
  else z = list(a, prop)
  return(z)
}

gibbs_chain(c(1,1), 30, 10, x_cond_y, y_cond_x)
gibbs_chain(c(1,1), 30, 100, x_cond_y, y_cond_x)
gibbs_chain(c(1,1), 30, 1000, x_cond_y, y_cond_x)
gibbs_chain(c(1,1), 30, 10000, x_cond_y, y_cond_x)

#Problem 2
library(animation)
bivariate_gibbs = function(x,m,p, pause = FALSE){
  y = matrix(rep(0,2*m),m,2)
  y[1,] = x
  sum = 0
  counter = 0
  plot(-3:3, -3:3, type='n', xlab='x',  ylab='y')
  for(i in 2:m){
    y[i,1] = rnorm(1, p*y[i-1,2], sqrt(1-p^2)) #conditional dist. of x|y
    y[i,2] = rnorm(1, p*y[i,1], sqrt(1-p^2)) #conditional dist. of y|x
    if(i >= m / 2){
      points(y[i,1], y[i,2])
      sum = sum + sqrt(y[i,1]^2 + y[i,2]^2)
      counter = counter + 1
    }
    #if(i > 1){
     # lines(y[i,1],y[i,2])
    #}
    if(pause) ani.pause()
  }
  sum = sum / counter
  z = list(y,'expected value' = sum)
  return(z)
}

ani.options(interval = 0.1)

a = bivariate_gibbs(c(0,0), 50, 0.9, pause = TRUE)

a = bivariate_gibbs(c(0,0), 500, 0.9)

a = bivariate_gibbs(c(0,0), 2000, -.45)
a
a = bivariate_gibbs(c(0,0), 2000, .99)
