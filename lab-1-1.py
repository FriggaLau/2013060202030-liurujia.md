import threading
import time
import os

amount = 0
mutex = threading.Lock()
full = threading.Event()
empty = threading.Event()

empty.set()
full.clear()

class ProducerThread(threading.Thread):
	def __init__(self, pcname):
		threading.Thread.__init__(self)
		self.pcname = pcname

	def run(self):
		global amount
		global empty
		global full

		print "producer:%d started" % self.pcname
		while True:
			if mutex.acquire(1):
				if full.isSet():
					mutex.release()
					continue

			amount = amount + 1
			msg = "Producer%d:%d -> %d" % (self.pcname, amount-1, amount)
			print msg

			empty.clear()
			if amount == 10:
				full.set()
			
			mutex.release()


class ConsumerThread(threading.Thread):
	def __init__(self, csname):
		threading.Thread.__init__(self)
		self.csname = csname

	def run(self):
		global amount
		global empty
		global full

		print "consumer:%s started" % self.csname

		while True:
			if mutex.acquire(1):
				if empty.isSet():
					mutex.release()
					continue

			amount = amount - 1
			msg = "Consumer%d:%d -> %d" % (self.csname, amount+1, amount)
			print msg

			full.clear()
			if amount == 0:
				empty.set()

			mutex.release()

class StopThread(threading.Thread):
	def __init__(self, secs):
		threading.Thread.__init__(self)
		self.secs = secs

	def run(self):
		while self.secs:
			self.secs -= 1
			time.sleep(1)

		os._exit(0)
		

if __name__ == '__main__':
	amount = int(raw_input('input # of consumber and producer:'))

	for x in range(amount):
		cs = ConsumerThread(x)
		pt = ProducerThread(x)
		cs.start()
		pt.start()

	st = StopThread(1)
	st.start()