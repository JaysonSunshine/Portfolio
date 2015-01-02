library(hash)

#functions
gene <- function(n){ #min exon length is 30
  size = rnorm(1,150,35)
  while(size < 30){
    size = rnorm(1,150,35)
  }
  a = list(sample(c("A", "C", "G", "T"),size, replace = TRUE))
  if(n > 1){
    for(i in 2:n){
      size = rnorm(1,150,35)
      while(size < 30){
        size = rnorm(1,150,35)
      }
      a[i] = list(sample(c("A", "C", "G", "T"),size, replace = TRUE))
    }
  }
  return(a)
}

genome <- function(n){
  a = list(gene(10))
  if(n > 1){
    for(i in 2:n){
      a[i] = list(gene(10))
    }
  }
  return(a)
}

reads <- function(genome, y, fusion_exon = FALSE){
  if(fusion_exon == FALSE){
    n = sample(1:length(genome), y, replace = TRUE) #sample a gene
    a = list(unlist(genome[n[1]]))
    if(y > 1){
    for(i in 2:y){
      a[i] = list(unlist(genome[n[i]]))
    }
  }
  }
  else{
    valid = TRUE
    while(valid){
      n = sample(0:(size - 1), 1)
      outer = max(ceiling(n/10), 1)
      inner = mod(n, 10) + 1
      if(genome[[outer]][inner] != 'X'){
        a = list(genome[[outer]][inner])
        valid = FALSE
      }
    }
    for(i in 2:y){
      valid = TRUE
      while(valid){
        n = sample(0:(size - 1), 1)
        outer = max(ceiling(n/10), 1)
        inner = mod(n, 10) + 1
        if(genome[[outer]][inner] != 'X'){
          a[i] = list(genome[[outer]][inner])
          valid = FALSE
        }
      }
    }
  }
  return(a)
}

fusion_genes <- function(genome, y){
  genes = vector()
  for(i in 1:y){
    n = integer()
    counter = integer()
    found_n = FALSE
    found = FALSE
    while(!found_n){
    n = sample(1:(length(genome) - 1), 1) #length of genome is 1?
    while(length(genome[[n]][[1]]) == 1){
      n = sample(1:(length(genome) - 1), 1) #length of genome is 1?
    }
    counter = 1
    while(!found && (n + counter <= length(genome))){
      if(length(genome[[n + counter]][[1]]) == 1) counter = counter + 1
      else found = TRUE
    }
    if(found) found_n = TRUE
    }
    genes = append(genes, c(n, n + counter))
    m_len = length(genome[[n]])
    m = sample(1:m_len, 1)
    m_next_len = length(genome[[n + counter]])
    m_next = sample(1:m_next_len, 1)
    if(m != m_len){
      a = genome[[n]][m_len - m + 1]
      genome[[n]][m_len - m + 1] = NULL
      if(m > 1){
        for(i in 2:m){
          a[i] = genome[[n]][m_len - m + 1]
          genome[[n]][m_len - m + 1] = NULL
        }
      }
    }
    else{
      a = genome[[n]][1]
      if(m_len > 1){
        for(i in 2:m_len){
          a[i] = genome[[n]][i]
        }
      }
      genome[[n]] = 'X' #used to be NULL
    }
    if(m_next != m_next_len){
      for(i in 1:m_next){
        a[i + m] = genome[[n + counter]][1]
        genome[[n + counter]][1] = NULL
      }
    }
    else{
      for(i in 1:m_next_len){
        a[m + i] = genome[[n + counter]][i]
      }
      genome[[n]] = 'X' #used to be NULL
    }
    genome[[length(genome) + 1]] = a
  }
  size = length(genome)
  for(i in size:1){
    if(length(genome[[i]][[1]]) == 1) genome[[i]] = NULL
  }
  genes = as.integer(unique(sort(genes)))
  z = list(genome, genes)
  return(z)
}

fusion_exons <- function(genome, y){ #fuses neighboring exons
  size = length(genome) * 10 #generalize
  for(i in 1:y){ #y is how many fusion events
    valid = TRUE
    counter = 0
    while(valid && counter < 1000){
      n = sample(0:(size - 2),1)
      outer = max(ceiling(n/10), 1)
      inner = mod(n, 10) + 1
      if(genome[[outer]][inner] != "X"){
        if(inner != 10){
          if(genome[[outer]][inner + 1] != "X"){
            valid = FALSE
          }
        }
        else{
          if(genome[[outer + 1]][1] != "X"){ #don't have to check case where outer out of                                 bounds since only sampled from 0:998
            valid = FALSE
          }
        }
      }
      counter = counter + 1
    }
    if(counter == 1000) return(genome)
    if(inner != 10){
      genome[[outer]][inner] = list(append(unlist(genome[[outer]][inner]),unlist(genome[[outer]][inner + 1])))
      genome[[outer]][inner + 1] = list('X')
    }
    else{
      genome[[outer]][inner] = list(append(unlist(genome[[outer]][inner]),unlist(genome[[outer + 1]][1])))
    }
  }
  return(genome)
}

hash_exons <- function(genome){
  h = hash()
  for(i in 1:length(genome)){
    for(l in 1:length(genome[[i]])){
      a = character()
      for(m in 1:15){
        a[m] = genome[[i]][[l]][m]
      }
      h[[paste(a, collapse = "")]] = append(h[[paste(a, collapse = "")]], i)
      b = character()
      for(m in 1:15){
        b[m] = genome[[i]][[l]][length(genome[[i]][[l]]) - 15 + m]
      }
      h[[paste(b, collapse = "")]] = append(h[[paste(b, collapse = "")]], i)
    }
  }
  return(h)
}

hash_exons_adv <- function(genome){
  h = hash()
  for(i in 1:length(genome)){
    for(l in 1:length(genome[[i]])){
      for(m in 1:(length(genome[[i]][[l]]) - 9)){
        h[[substring(paste(genome[[i]][[l]], collapse = ""), 0 + m, 9 + m)]] = append(h[[substring(paste(genome[[i]][[l]], collapse = ""), 0 + m,9 + m)]], i)
      }
    }
  }
  return(h)
}

scan_exons_baseline <- function(coverage_genes, h_hash){
  fusion = vector()
  genes = vector()
  for(i in 1:length(coverage_genes)){
    a = character()
    for(l in 1:10){
      a[l] = coverage_genes[[i]][l]
    }
    exon_1 = h_hash[[paste(a, collapse = "")]]
    b = character()
    for(m in 1:10){
      b[m] = coverage_genes[[i]][length(coverage_genes[[i]]) - 10 + m]
    }
    exon_2 = h_hash[[paste(b, collapse = "")]]
    if(length(exon_2) > 1 || length(exon_1) > 1){
      genes = append(genes, c(exon_1, exon_2))
    }
    else if(exon_2 != exon_1){
      fusion[i] = TRUE
      genes = append(genes, c(exon_1, exon_2))
    }
    else fusion[i] = FALSE
  }
  genes = as.integer(unique(sort(genes)))
  z = list(fusion, genes)
  return(z)
}

scan_exons_adv <- function(coverage_genes, h_hash){
  fusion = vector()
  genes = vector()
  for(i in 1:length(coverage_genes)){
    for(m in 1:floor((length(coverage_genes[[i]]) / 10))){
      genes = append(genes, h_hash[[substring(paste(coverage_genes[[i]], collapse = ""), 10*m - 9, 10*m)]])
    }
    genes = unique(Filter(function (elem) length(which(genes == elem)) > 4, genes))
    if(length(genes) > 1) fusion = append(fusion, genes)
  }
  return(sort(unique(fusion)))
}
    
test_genome <- function(n, m, genome, h_hash, adv = FALSE){
  if(!adv){
    c_a_false_exons = vector()
    missed_exons = vector()
    for(i in 1:n){
      a_test = list()
      b_test = list()
      c_test = list()
      output_a_c = list()
      output_c_a = list()
      a_test = fusion_genes(genome, 10)
      b_test = reads(a_test[[1]], m)
      c_test = scan_exons_baseline(b_test, h_hash)
      output_a_c = setdiff(a_test[[2]], c_test[[2]])
      output_c_a = setdiff(c_test[[2]], a_test[[2]])

      c_a_false_exons[i] = length(output_c_a) / length(c_test[[2]])
      missed_exons[i] = length(output_a_c) / length(a_test[[2]])
    }
    z = list(missed_exons, mean(missed_exons), c_a_false_exons, mean(c_a_false_exons))
    return(z)
  }
  else{
    c_a_false_exons = vector()
    missed_exons = vector()
    for(i in 1:n){
      a_test = list()
      b_test = list()
      c_test = list()
      output_a_c = list()
      output_c_a = list()
      a_test = fusion_genes(genome, 10)
      b_test = reads(a_test[[1]], m)
      c_test = scan_exons_adv(b_test, h_hash)
      output_a_c = setdiff(a_test[[2]], c_test)
      output_c_a = setdiff(c_test, a_test[[2]])
      missed_exons[i] = length(output_a_c) / length(a_test[[2]])
      c_a_false_exons[i] = length(output_c_a) / length(c_test)
    }
  }
  z = list(output_a_c, output_c_a, missed_exons, mean(missed_exons), c_a_false_exons, mean(c_a_false_exons))
  return(z)
}
      
x = c(1,10,100)
z = c(1,2,5,10)
y_naive = c(.408, .066, .056)
y_adv = c(.365, .206, 0.048, 0.049)
y_naive_fp = c(0.021, 0.021, 0.036)
y_adv_fp = c(0,0,0,0)
plot(x_naive,y_naive,col = "blue", log ="x", main = 'Missed Fusion Gene Rate', xlab = "Coverage", ylab = 'Missed Fusion Gene Rate', ylim = c(0, 0.41), pch = 19)
legend(20, 0.3, c("Naive", "Advanced"), lty=c(1,1), col = c("blue", "red"))
lines(x,y_naive, col = "blue")
points(z, y_adv, col = 'red', pch = 19)
lines(z,y_adv, col = 'red')
x_adv = c(100,200,1000)
y_adv = c(.365, .206, .049)

#simulation
g = genome(100)
h_naive = hash_exons(g)
h_adv = hash_exons_adv(g)

test_genome(10,500, g, h_adv)
