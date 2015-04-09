
#dummy client
require 'socket'


puts "1 for remote 2 for localhost:"
select = gets
if select.to_i == 1
  hostname = '155.98.111.62'
else
  hostname = 'localhost'
end
puts "Hostname: #{hostname}"

puts "Enter a port:"
port = gets

puts "Port: #{port}"

s = TCPSocket.open(hostname, port.to_i)
puts "Attempting to connect..."

Thread.new{
  while true
    puts s.gets
  end
}
while true
  message = gets
  s.puts message
end

