#dummy client
require 'socket'
require 'test/unit'

class ServerTest < Test::Unit::TestCase

  #Open the client connection to server each time
  def setup
    #@s = TCPSocket.open("155.98.111.62", 2120)
    @s = TCPSocket.open("localhost", 2118)
    @x = TCPSocket.open("localhost", 2118)
  end

  #close the socket after each test
  def teardown
    @s.close
    @x.close
  end

  #test that client receives test connection
  def test_connection
    @s.puts "connect colin spreadsheet"
    @line = @s.gets
    assert_match(/connected \d/ ,@line)
  end

  #should get an error message because client is not registered
  def test_not_registered
    @s.puts "connect gregory spreadsheet"
    @line = @s.gets
    assert_match(/error 4 gregory/ ,@line)
  end

  #send a simple register message
  def test_register
    @s.puts "connect sysadmin spreadsheet"
    @line = @s.gets
    assert_match(/connected \d/ ,@line)
    @s.puts "register tommy"
  end

  def test_multiple_users   
      @s.puts "connect sysadmin spreadsheet"
      @x.puts "connect sysadmin spreadsheet"
      sleep(5)   
    assert_equal(@s.gets,@x.gets)
  end
end
