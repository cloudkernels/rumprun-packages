import sys

print('\n\nInitial modules\n\n')
for k in sorted(sys.modules.keys()):
        print(k)



moduleFiles = [module.__file__ for module in sys.modules.values()
                  if hasattr(module, '__file__') and module.__file__ != '<frozen>']

moduleFiles.sort()

print('\n\npython modules loaded from filesystem (.py or .pyc):\n\n')
for filename in moduleFiles:
    if filename.endswith('.py') or filename.endswith('.pyc'):
        print(filename)

print('\n\npython C extension modules loaded from filesystem (.so):\n\n')
for filename in moduleFiles:
    if filename.endswith('.so'):
        print(filename)
