import subprocess

cmd = ["ps", "-ajx"]
res = subprocess.run(cmd, shell=True, capture_output=True, text=True).stdout

daemon_lines = list(filter(lambda s: 'main.out' in s, res.split('\n')))
if daemon_lines:
    d = daemon_lines[0]
    print(int(d.split()[0]))
else:
    print('daemon not found.')
