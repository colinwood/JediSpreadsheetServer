#dummy client
require 'socket'

hostname = '155.98.111.62'

20.times do
	Thread.new{
		s = TCPSocket.open(hostname, 2120)
		puts "Attempting to connect..."


		Thread.new{
		  while true
		    puts s.gets
		  end
		}

		Thread.new{
			200.times do
			message = "Apples\n"
			s.puts message
			end
		}

		Thread.new{
			200.times do
			message = "Oranges\n"
			s.puts message
			end
		}
		s.close
	}
end
gets


