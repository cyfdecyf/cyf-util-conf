#!/usr/bin/env ruby

prev_empty = false

ARGF.each_line do |line|
  if line == $/
    # always output newline if it's the only thing in a line
    puts
    if not prev_empty
      # add newline to the last line of a paragraph
      puts
    end
    prev_empty = true
  else
    if line =~ /^[:space:]/
      print line
    else
      print line.chomp
    end
    prev_empty = false
  end
end
