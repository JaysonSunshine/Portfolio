#Problems 1 and 2: use transition matrices and eigenvalues?

#Problem 3

#3.1
B = matrix(c(0,0.5,0.5,.5,0,0.5, 0.5, 0.5, 0), 3, 3)

#3.2
acc = matrix(c(1,.5, 1, 1, 1, 1, 1, .5, 1), 3, 3, byrow = TRUE)
m = matrix(c(.25,.25, .5, .5, 0, .5, .5, .25, .25), 3, 3, byrow = TRUE)

#3.3
trans_exact = function(x, t, m, output = FALSE, plot = FALSE){
  p = matrix(rep(0, 3*t), t, 3)
  if(x == 1){
    p[1,] = c(1,0,0)
    #r[1,] = p[1,]
  }
  else if(x == 2){
    p[1,] = c(0,1,0)
    #r[1,] = p[1,]
  }
  else{
    p[1,] = c(0,0,1)
    #r[1,] = p[1,]
  }
  for(i in 2:t){
    p[i,] = p[i - 1,] %*% m
  }
  if(plot == TRUE){
    print_y = c(rep(0, t))
    for(l in 1:t){
      #y = rowSums(total[l,,]) / n
      #print_y[l] = max(y - 1/3)
      print_y[l] = sum(abs(p[l] - 1/3))
    }
    plot(print_y)
  }
  return(p)
}

trans_exact(1,10,m)
trans_exact(2,10,m)
trans_exact(3,10,m)

#3.4
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
      #sample(c(1,2,3), 1, prob = m[1,])
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
    #print_y = c(rep(0, t))
    for(l in 1:t){
      y = rowSums(total[l,,]) / n
      #print_y[l] = max(y - 1/3)
      print_y[l] = sum(abs(y - 1/3))
    }
    plot(print_y)
  }
  if(output == TRUE) z = list(total = total,w,rate, print_y)
  else z = list(w,rate)
  return(z)
}

random_w2(1,m,25,1e4)
random_w2(2,m,25,1e4)
random_w2(3,m,25,1e4)

#3.5

#x starting state, m transition matrix, t time steps, n number of simulations, z number of last time steps in which to monitor population movement between states

random_w2 = function(x, m, t, n, z, output = FALSE, plot = FALSE){
  w = c(0,0,0)
  move = matrix(c(rep(0, 9)), 3, 3)
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
      #sample(c(1,2,3), 1, prob = m[1,])
      p[i,] = r[i - 1,] %*% m
      y = sample(c(1,2,3), 1, prob = p[i,])
      if(y == 1) r[i,] = c(1,0,0)
      else if(y == 2) r[i,] = c(0,1,0)
      else r[i,] = c(0,0,1)
      if(t - i < z){
        a = r[i,] - r[i - 1,]
        for(v in 1:length(a))
        {
          if(a[v] == -1){
            move[v,y] = move[v,y] + 1
          }
        }
      }
    }
    w = w + r[i,]
    total[,,l] = r
  }
  rate = c(w[1]/sum(w), w[2]/sum(w), w[3]/sum(w))
  print_y = c(rep(0, t))
  
  if(plot == TRUE){
    #print_y = c(rep(0, t))
    for(l in 1:t){
      y = rowSums(total[l,,]) / n
      #print_y[l] = max(y - 1/3)
      print_y[l] = sum(abs(y - 1/3))
    }
    plot(print_y)
  }
  move_ratio = move
  move_ratio[2,1] = move[2,1] / move[1,2]
  move_ratio[1,2] = 1
  move_ratio[3,1] = move[3,1] / move[1,3]
  move_ratio[1,3] = 1
  move_ratio[3,2] = move[3,2] / move[2,3]
  move_ratio[2,3] = 1
  if(output == TRUE) z = list(total = total,w,rate, print_y, move)
  else z = list(distribution = w, distribution_rate = rate, move = move, move_ratio = move_ratio)
  return(z)
}


nsim = 1e4
random_w2(1,m,30,nsim,10)
random_w2(2,m,30,nsim,10)
random_w2(3,m,30,nsim,10)

#Problem 4
#4.1
place_cities = function(m){
  cities = matrix(c(rep(0, 2 * (m + 1))), m + 1,2)
  for(i in 0:m){
    cities[i + 1,1] = cos(pi - (i*2*pi)/(m + 1)) + 1
    cities[i + 1,2] = sin((i*2*pi)/(m + 1))
  }
  return(cities)
}
cities = place_cities(7)
cities
cities_dist = as.matrix(dist(cities))
cities_dist

#4.2
path_distance = function(cities, path){
  distance = 0
  for(i in 1:(length(path) - 1)){
    cities_dist = as.matrix(dist(cities))
    new_dist = cities_dist[path[i] + 1 , path[i + 1] + 1]
    distance = distance + new_dist
  }
  return(distance)
}

generate_path = function(cities){
  y = sample(1:(dim(cities)[1] - 1), dim(cities)[1] - 1)
  path = c(0,y,0)
  return(path)
}

plot_path = function(cities, path,color){
  m = matrix(c(rep(0,2*length(path))), length(path),2)
  for(i in 1:length(path)){
    m[i,] = cities[path[i] + 1,]
  }
  lines(m, col = color)
  return(m)
}


#red path is initial path, blue path is bestpath
trav_salesman = function(m, T_value, Nsim, box = FALSE, sim_ann = FALSE, plot = FALSE, plot_pathlength = FALSE){
  rate = (0.1 / T_value) ^ (1 / Nsim)
  if(box) cities = place_cities_box(m)
  else cities = place_cities(m)
  if(plot){
    plot(cities)
  }
  path = generate_path(cities)
  initial_path = path
  if(plot) plot_path(cities, path, "red")
  u = path_distance(cities, path)
  initial_distance = u
  r = u
  bestpath = path
  for(i in 2:Nsim){
    y = sample(2:(length(path) - 1),2)
    pathtemp = path
    temp = pathtemp[y[1]]
    pathtemp[y[1]] = pathtemp[y[2]]
    pathtemp[y[2]] = temp
    a = min(1,exp(-path_distance(cities,pathtemp) / T_value) / exp(-u / T_value))
    if(runif(1) < a){
      u = path_distance(cities, pathtemp)
      path = pathtemp
      if(u < path_distance(cities, bestpath)) bestpath = path
    }
    r[i] = path_distance(cities,path)
    if(sim_ann) T_value = T_value * rate
  }
  if(plot) plot_path(cities, bestpath, "blue")
  z = list(initial_path = initial_path, initial_distance = initial_distance, bestpath = bestpath, bestpath_distance = path_distance(cities, bestpath))
  if(plot_pathlength) plot(r)
  return(z)
}

trav_salesman(20,5,500, plot = TRUE, plot_pathlength = TRUE)
trav_salesman(20,.5,500, plot = TRUE, plot_pathlength = TRUE)
trav_salesman(20,.05,500, plot = TRUE, plot_pathlength = TRUE)

#4.3
trav_salesman(20,5,500, sim_ann = TRUE, plot = TRUE, plot_pathlength = TRUE)

#4.4
place_cities_box = function(m){
  cities = matrix(c(rep(0, 2* (m + 1))), m + 1, 2)
  for(i in 2:(m + 1)){
    cities[i, 1] = runif(1)
    cities[i, 2] = runif(1)
  }
  return(cities)
}

trav_salesman(20,5,500,sim_ann = TRUE, box = TRUE, plot = TRUE, plot_pathlength = TRUE)