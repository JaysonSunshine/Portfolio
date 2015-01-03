#length(omega) = number of possible paths with lenth n 
#seuential importance sampling
  #1.start x1 = (0,0), w1 = 1
  #2. nt is possibilities at step t
  #3 w(x) = 1/g(x) = 1/(1/n1*1/n2*1/nN-1) = prod(ni, i, 1, N-1) #represents one path
#length(omega_hat) 1/m*sum(w, i, 1, m), #i.e. do previous procedure m times
#length(omega)

#Problem 1
random_w = function(n, m){
  choice = 1
  lens = c(rep(0,m))
  w = c(rep(1,m))
  for(l in 1:m){
    a = matrix(rep(FALSE,(2*n + 1)^2), 2*n + 1, 2*n + 1)
    x = n + 1
    y = n + 1
    a[x,y] = TRUE
    i = 0
    opt = NULL
  while(i < n){
    if(a[x - 1,y] == FALSE) opt = append(opt,1)
    if(a[x,y + 1] == FALSE) opt = append(opt,2)
    if(a[x + 1,y] == FALSE) opt = append(opt,3)
    if(a[x,y - 1] == FALSE) opt = append(opt,4)  
    if(length(opt) > 0){
      choice = sample(opt,1)
      w[l] = w[l] * length(opt)
    }
    else{
      break
    }
    if(choice == 1) x = x - 1
    else if(choice == 2) y = y + 1
    else if(choice == 3) x = x + 1
    else y = y - 1
    a[x,y] = TRUE
    opt = NULL  
    i = i + 1
  }
  lens[l] = i
  }
  p = lens
  p[p < n] = 0 #indicator function
  p[p == n] = 1
  z = list(score = 1/m * sum(p * w), paths = p)
  return(z)
}

random_w(10,10)

random_w(100,10)
random_w(1000,10) #the portion of paths that don't reach length n increases to 1

#Problem 2
trans_exact = function(x, t, n = 1, output = FALSE, plot = FALSE, m11 = 0.5, m12 = 0.25, m13 = 0.25, m21 = .25, m22 = .5, m23 = .25, m31 = 0.25, m32 = 0.25, m33 = 0.5){
  m = matrix(c(m11, m12, m13, m21, m22, m23, m31, m32, m33), byrow = TRUE, 3, 3)
  p = matrix(rep(0, 3*t), t, 3)
  if(x == 1){
    p[1,] = c(1,0,0)
  }
  else if(x == 2){
    p[1,] = c(0,1,0)
  }
  else{
    p[1,] = c(0,0,1)
  }
  for(i in 2:t){
    p[i,] = p[i - 1,] %*% m
  }
  if(plot == TRUE){
    print_y = c(rep(0, t))
    for(l in 1:t){

      print_y[l] = sum(abs(p[l] - 1/3))
    }
    plot(print_y)
  }
  return(p)
}

trans_exact(1,5, plot = TRUE)
trans_exact(2,5, plot = TRUE)
trans_exact(3,5, plot = TRUE)
trans_exact(1,30, plot = TRUE)
trans_exact(2,30, plot = TRUE)
trans_exact(3,30, plot = TRUE)

#Problem 3
m = matrix(c(.5,.25,.25,.25,.5,.25,.25,.25,.5), byrow = TRUE, 3, 3)
random_w2 = function(x, m, t, n, output = FALSE, plot = FALSE){
  w = c(0,0,0)
  total = array(dim=c(t,3,n))
  for(l in 1:n){
    p = matrix(rep(0, 3*t), t, 3)
    r = matrix(rep(0, 3*t), t, 3)
    if(x == 1){
      p[1,] = c(1,0,0)
      r[1,] = p[1,]
    }
    else if(x == 2){
      p[1,] = c(0,1,0)
      r[1,] = p[1,]
    }
    else{
      p[1,] = c(0,0,1)
      r[1,] = p[1,]
    }
    for(i in 2:t){
      p[i,] = r[i - 1,] %*% m
      y = sample(c(1,2,3), 1, prob = p[i,])
      if(y == 1) r[i,] = c(1,0,0)
      else if(y == 2) r[i,] = c(0,1,0)
      else r[i,] = c(0,0,1)
    }
    w = w + r[i,]
    total[,,l] = r
  }
  rate = c(w[1]/sum(w), w[2]/sum(w), w[3]/sum(w))
  print_y = c(rep(0, t))
  
  if(plot == TRUE){
    for(l in 1:t){
      y = rowSums(total[l,,]) / n
      print_y[l] = sum(abs(y - 1/3))
    }
    plot(print_y)
  }
  if(output == TRUE) z = list(total = total,w,rate, print_y)
  else z = list(w,rate)
  return(z)
}

random_w2(1,500, 50, plot = TRUE)
random_w2(2,500, 50, plot = TRUE)
random_w2(3,500, 50, plot = TRUE)

N = 1:10
walks = numeric(length(N))
Nsim = 1e4
for(n in N) #If you don't have much RAM, this could take some time. Reduce Nsim or N
  {
    w = replicate(Nsim, random_w(n,1)[1])
    walks[n] = mean(w)
    }
library(ggplot2)
library(scales)
ggplot(data.frame(N, walks), aes(x=N, y=walks)) + geom_point(size=3) +
  geom_line() + theme_minimal() + scale_y_continuous('# SAWs', labels=comma)