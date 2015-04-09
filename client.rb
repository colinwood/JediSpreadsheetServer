require 'socket'     



hostname = '155.98.111.62'

port = 2120

s = TCPSocket.open(hostname, port)
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
            