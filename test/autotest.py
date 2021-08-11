import subprocess
import sys
import os

cmds = ["python3", "src/main.py", "--mode", "mod", "--path", "test/example.json"]

def autotest():
  success, fail, total = 0, 0, 0
  with open('test/code.txt', 'r') as f:
    codes = f.read()
    codes = codes.split('==========================')
  idx = 0
  if os.path.exists('test/result.txt'):
    os.remove('test/result.txt')
  fres = open('test/result.txt', 'a')
  total = len(codes)
  for code in codes:
    print(f"testing {idx}....")
    with open('test/test.py', 'w') as fw:
      fw.write(code)
      fw.flush()
      try:
        popen = subprocess.Popen(cmds, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        stdout, _ = popen.communicate(timeout=10)
        stdout = stdout.decode()
        success += 1
        fres.write(stdout)
        fres.write("\n>>>>>>>>>>\n")
        fres.flush()
      except:
        sys.stdout.flush()
        popen.kill()
        fail += 1
        fres.write(f"error: {idx}\n")
        fres.write(code)
        fres.write("\n>>>>>>>>>>\n")
        fres.flush()
    idx += 1
  print(f"total case: {total}, success: {success}/{total}, fail: {fail}/{total}")

autotest()
        