#dummy client
require 'socket'

hostname = 'localhost'


s = TCPSocket.open("155.98.111.62", 2119)
#s = TCPSocket.open("localhost", 2119)
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
