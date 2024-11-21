#this is a dummy py process so we can start async on windows
#properly without stuff like wsl or dbs or cache or anything else than php and python
import sys
from subprocess import Popen

if len(sys.argv) > 1:
	id = sys.argv[1]
else:
	print("false")
	exit()

Popen(['python', 'exec.py', id]) # something long running
# subprocess.Popen(['python', 'exec.py', id])

print("true")

