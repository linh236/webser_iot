require 'dino'

board = Dino::Board.new(Dino::TxRx.new)
led1 = Dino::Components::Led.new(pin: 13, board: board)
led2 = Dino::Components::Led.new(pin: 14, board: board)
led3 = Dino::Components::Led.new(pin: 15, board: board)

		# [:on, :off].cycle do |switch|
		# 		led.send(switch)
		# 		sleep 1
		# 		led1.send(switch)
		# 		sleep 1
		# 		led2.send(switch)
		# 		sleep 1
		# 	end
def blink(led, active)
		if active == 1
					led.send(:on)
		elsif active == 2
			[:on,:off].cycle do |switch|
				led.send(switch)
				sleep(0.1)
				led.send(switch)
				sleep(0.1)
			end
		end
end

puts "Chon den led sang: ";
leds = gets
puts "Chon che do sang: ";
turn = gets
if leds.to_i == 1
	puts "Ctr + z : turn off";
	blink(led1,turn.to_i);
elsif leds.to_i == 2
	puts "Ctr + z : turn off";
	blink(led2,turn.to_i);
else
	puts "Ctr + z : turn off";
	blink(led3,turn.to_i);
end
