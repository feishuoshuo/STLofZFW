import os
import subprocess
import stat

def create_build_directory(build_dir):
    """
    创建 build 目录（如果不存在）。
    """
    if not os.path.exists(build_dir):
        print(f"Creating build directory: {build_dir}")
        os.makedirs(build_dir)
    else:
        print(f"Build directory already exists: {build_dir}")

def configure_project(build_dir):
    """
    在 build 目录中运行 cmake 配置项目。
    """
    print(f"Configuring project in {build_dir}...")
    try:
        subprocess.run(["cmake", ".."], cwd=build_dir, check=True)
        print("Project configured successfully.")
    except subprocess.CalledProcessError as e:
        print(f"Configuration failed: {e}")
        exit(1)

def build_project(build_dir):
    """
    在 build 目录中运行 make 构建项目。
    """
    print(f"Building project in {build_dir}...")
    try:
        subprocess.run(["make"], cwd=build_dir, check=True)
        print("Build completed successfully.")
    except subprocess.CalledProcessError as e:
        print(f"Build failed: {e}")
        exit(1)

def find_executables(bin_dir):
    """
    在指定目录中查找所有可执行文件。
    """
    print(f"Searching for executables in {bin_dir}...")
    executables = []
    if os.path.exists(bin_dir):
        for entry in os.listdir(bin_dir):
            full_path = os.path.join(bin_dir, entry)
            if os.path.isfile(full_path) and os.access(full_path, os.X_OK):
                executables.append(full_path)
    else:
        print(f"Directory {bin_dir} does not exist.")
    return executables

def run_executables(executables):
    """
    逐个运行可执行文件。
    """
    print("Running executables...")
    for exe in executables:
        print(f"Executing: {exe}")
        try:
            result = subprocess.run([exe], check=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
            print(f"Output:\n{result.stdout}")
            if result.stderr:
                print(f"Error:\n{result.stderr}")
        except subprocess.CalledProcessError as e:
            print(f"Execution failed for {exe}: {e}")
        except Exception as e:
            print(f"Error running {exe}: {e}")

def main():
    build_dir = "build"
    bin_dir = "bin"  # 假设 bin 文件夹和 build 文件夹处于同一路径下

    # 创建 build 目录（如果不存在）
    create_build_directory(build_dir)

    # 配置项目
    configure_project(build_dir)

    # 构建项目
    build_project(build_dir)

    # 查找 bin 目录中的可执行文件
    executables = find_executables(bin_dir)
    if not executables:
        print("No executables found.")
        exit(1)

    # 运行可执行文件
    run_executables(executables)

if __name__ == "__main__":
    main()