################################################################
# CS124/Eskin                                   UCLA ID 504209098
# Date 22 Apr 2014
# Homework 2
##################################################################

#Problem 1
a = c(0.05, 0.01)
M = c(2, 5, 10, 100, 1000)
a = 0.05
sidak = 1 - (1 - a) ^ (1 / M)
bonferroni = a / M
bonferroni
sidak
a = 0.01
sidak = 1 - (1 - a) ^ (1 / M)
bonferroni = a / M
bonferroni
sidak

#Problem 2.1
p = 0.3
gamma = 2
p_a_pos = (gamma * p) / ((gamma - 1) * p + 1)
p_a_pos
p_a = (p_a_pos + p) / 2
p_a

N = 200
lambda = (p_a_pos - p) / sqrt(2 * p_a * (1 - p_a))
lambda
non_cent_0.3 = lambda * sqrt(N)

p = 0.15
p_a_pos = (gamma * p) / ((gamma - 1) * p + 1)
p_a_pos
p_a = (p_a_pos + p) / 2
p_a

lambda = (p_a_pos - p) / sqrt(2 * p_a * (1 - p_a))
lambda
non_cent_0.15 = lambda * sqrt(N)
non_cent_0.15

#corr_cof_0.3 = c(1,0.9,0.85,0.5,0.4)
#corr_cof_0.15 = c(1, 0.6, 0.75, 0.4, 0.3)
#non_cent_0.3_params = non_cent_0.3 * corr_cof_0.3
#non_cent_0.15_params = non_cent_0.15 * corr_cof_0.15

a = 0.05
power_0.3 = pnorm(qnorm(a / 20) + non_cent_0.3) + 1 - pnorm((-qnorm(a / 20) + non_cent_0.3))
power_0.3
power_0.15 = pnorm(qnorm(a / 20) + non_cent_0.15) + 1 - pnorm((-qnorm(a / 20) + non_cent_0.15))
power_0.15
total_power = (sum(power_0.3) + sum(power_0.15)) / 2

#Problem 2.2
corr_cof_0.15 = c(0.6, 1, 0.8, 0.85, 0.8)
non_cent_0.15_params = non_cent_0.15 * corr_cof_0.15
non_cent_0.15_params
corr_cof_0.3 = c(1, 0.9, 0.9, 0.85, 1)
non_cent_0.3_params = non_cent_0.3 * corr_cof_0.3
non_cent_0.3_params
power_0.3 = pnorm(qnorm(a / 6) + non_cent_0.3_params) + 1 - pnorm((-qnorm(a / 6) + non_cent_0.3_params))
power_0.3
power_0.15 = pnorm(qnorm(a / 6) + non_cent_0.15_params) + 1 - pnorm((-qnorm(a / 6) + non_cent_0.15_params))
power_0.15
total_power_corr = (sum(power_0.3) + sum(power_0.15)) / 10
total_power_corr

#Problem 2.3
corr_cof_0.15 = c(0.6, 1, 0.8, 0.85, 0.8)
non_cent_0.15_params = non_cent_0.15 * corr_cof_0.15
non_cent_0.15_params
corr_cof_0.3 = c(0.85, 0.95, 1, 0.65, 0.9)
non_cent_0.3_params = non_cent_0.3 * corr_cof_0.3
non_cent_0.3_params
power_0.3 = pnorm(qnorm(a / 4) + non_cent_0.3_params) + 1 - pnorm((-qnorm(a / 4) + non_cent_0.3_params))
power_0.3
power_0.15 = pnorm(qnorm(a / 4) + non_cent_0.15_params) + 1 - pnorm((-qnorm(a / 4) + non_cent_0.15_params))
power_0.15
total_power_corr = (sum(power_0.3) + sum(power_0.15)) / 10
total_power_corr

#Problem 3
snp_a = c(1,0,1,1,0,1,0,1,1,1)
snp_b = c(1,0,1,0,0,1,1,1,0,1)
cor(snp_a,snp_b)
p_a = 0.3
p_b = 0.4
p_ab = 0.2
cor_ab = (p_ab - p_a*p_b) / (sqrt(p_a*(1-p_a))*sqrt(p_b*(1-p_b)))
cor_ab

#Problem 3.2
ncp_b = (0.4 - 0.5) / (sqrt(2/1000) * sqrt(0.45 * (1 - 0.45)))
ncp_a = cor_ab * ncp_b
ncp_a
power_ncp_a = pnorm(qnorm(a / 2) + ncp_a) + 1 - pnorm((-qnorm(a / 2) + ncp_a))
power_ncp_a
