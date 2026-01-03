import subprocess
import shutil
import os
from pathlib import Path

def run(cmd):
  return subprocess.check_output(cmd, shell=True, text=True).strip()

def is_executable(path: Path) -> bool:
  return (path.is_file() and os.access(path, os.X_OK) and not path.name.startswith("."))

def getMetadata(bin_directory):
  short_sha = run("git rev-parse --short HEAD")
  commit_count = run("git rev-list HEAD --count")
  project_version = run(f"meson introspect --projectinfo {bin_directory} | jq -r '.version'")
  return (short_sha, commit_count, project_version)

def findAndCopyTestExecutables(tests_path, destination_directory):
  test_executables = [p for p in tests_path.rglob("*") if is_executable(p)]
  for exe in test_executables:
    print(f"Copying {exe} into {destination_directory}")
    shutil.copy2(exe, destination_directory / exe.name)

def createDependencyGraph(bin_directory, destination_directory):
  dot_file = destination_directory / "ku.dot"
  svg_file = destination_directory / "ku.svg"
  print(f"Creating dependency graph's dot file {dot_file}'")
  subprocess.run(f"ninja -C {bin_directory} -t graph > {dot_file}", shell=True, check=True)
  print(f"Creating dependency graph's svg file {svg_file}'")
  subprocess.run(f"dot -Gsplines=ortho -Tsvg {dot_file} -o {svg_file}", shell=True, check=True)

def main():
  bin_directory = Path("bin/Release")
  destination_base_directory = Path("out")
  test_root = bin_directory.joinpath('test')

  short_sha, commit_count, project_version = getMetadata(bin_directory);

  tests_destination_directory = destination_base_directory / "tests" / f"ku-{project_version}.{commit_count}.{short_sha}"
  print(f"Package Destination directory: {tests_destination_directory}")
  tests_destination_directory.mkdir(parents=True, exist_ok=True)
  findAndCopyTestExecutables(test_root, tests_destination_directory)

  dependency_graph_destination_directory = destination_base_directory / "dependency-graph"
  dependency_graph_destination_directory.mkdir(parents=True, exist_ok=True)
  createDependencyGraph(bin_directory, dependency_graph_destination_directory)

if __name__ == "__main__":
  main()