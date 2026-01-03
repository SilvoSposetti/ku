from pathlib import Path
import subprocess
import os

def is_executable(path: Path) -> bool:
  return (path.is_file() and os.access(path, os.X_OK) and not path.name.startswith("."))

def findUnitTestExecutables(tests_path):
  executables = [p for p in tests_path.rglob("*") if is_executable(p)]
  return [p for p in executables if "PerformanceTest" not in p.name]

def main():
  basePath = Path(".");
  input_directory = next((p.resolve() for p in Path(".").glob("ku-*") if p.is_dir()), None)
  reports_directory = Path("reports")
  reports_directory.mkdir(parents=True, exist_ok=True)

  tests = findUnitTestExecutables(input_directory)
  for test in tests:
    test_path = Path(test)

    test_path.chmod(0o755) # Ensure executable permissions

    # Output file name
    report_path = reports_directory / f"{test.name}_report.xml"
    print(f"Running {test} and outputting to {report_path}")

    # Run the test executable
    subprocess.run(
        [str(test_path), "--success=true", "--reporters=junit", f"--out={report_path}"],
        check=True
    )


if __name__ == "__main__":
  main()
