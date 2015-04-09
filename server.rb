require 'socket'                # Get sockets from stdlib

puts "TEST ECHO SERVER"
print "Enter a port to allow connections on: "
port = gets
server = TCPServer.open(port.to_i)

puts "Accepting connections on \n PORT: #{port}"
loop do
  Thread.start(server.accept) do |client|
    puts "Client connected..."
    client.puts "You are connected"
    loop do
      message = client.gets
      puts message
      client.puts message
    end
  end
end
