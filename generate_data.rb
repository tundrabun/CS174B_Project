out_file = File.new("input_data.txt", "w")
lower_limit = 1 
num_entries = ARGV[0].to_i
upper_limit = ARGV[1].to_i
random_nums = (0...upper_limit).to_a.shuffle.take(num_entries)
random_rids = (0...upper_limit).to_a.shuffle.take(num_entries)

for random_num in random_nums
    out_file.puts random_num.to_s + " " + random_num.to_s
end
