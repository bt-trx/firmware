import os
import subprocess

Import("env")
print("Pre build scripts")

# Return the git revision as a string
def git_version():
    def _minimal_ext_cmd(cmd):
        # construct minimal environment
        env = {}
        for k in ['SYSTEMROOT', 'PATH']:
            v = os.environ.get(k)
            if v is not None:
                env[k] = v
        # LANGUAGE is used on win32
        env['LANGUAGE'] = 'C'
        env['LANG'] = 'C'
        env['LC_ALL'] = 'C'
        out = subprocess.Popen(cmd, stdout = subprocess.PIPE, env=env, shell=True).communicate()[0]
        return out

    try:
        out = _minimal_ext_cmd("git describe --tags")
        GIT_REVISION = out.strip().decode('ascii')
    except OSError:
        GIT_REVISION = "Unknown"

    return GIT_REVISION

print("GIT Version: " + git_version())

with open("src/git_revision.h", "w") as text_file:
    text_file.write("const char* GIT_REVISION = \"{0}\";".format(git_version()))
