# Read input in format: city, state, population (tab delimited file. 
# population is of form: integer number always followed by a 'k'). 
# Note that city name can contain spaces. 
# Output the total population in each state in descending order

awk -F'\t' '{print $2, $3}'|sort |
cut -c 1- |
awk -F'\t' '{a[$1] += $2} END{ for(k in a) print k,a[k]}' | 
sort -k 2 -nr
