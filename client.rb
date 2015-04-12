#dummy client
require 'socket'

hostname = 'localhost'


s = TCPSocket.open(hostname, 2119)
puts "Attempting to connect..."


Thread.new{
  while true
	puts s.gets
  end
}

while true
	line = gets
	s.puts line
end



