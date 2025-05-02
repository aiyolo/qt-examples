
import os
from conans.util.files import rmdir, mkdir
from conan.errors import ConanException
import shutil

def _deploy_single(dep, conanfile, output_folder, folder_name):
    new_folder = os.path.join(output_folder, folder_name)
    rmdir(new_folder)
    symlinks = conanfile.conf.get("tools.deployer:symlinks", check_type=bool, default=True)
    try:
        shutil.copytree(dep.package_folder, new_folder, symlinks=symlinks)
    except Exception as e:
        if "WinError 1314" in str(e):
            ConanOutput().error("full_deploy: Symlinks in Windows require admin privileges "
                                "or 'Developer mode = ON'", error_type="exception")
        raise ConanException(f"full_deploy: The copy of '{dep}' files failed: {e}.\nYou can "
                             f"use 'tools.deployer:symlinks' conf to disable symlinks")
    dep.set_deploy_folder(new_folder)

def deploy(graph, output_folder):
    output_folder = os.path.join(output_folder)
    conanfile = graph.root.conanfile
    conanfile.output.info(f"Conan built-in pkg deployer to {output_folder}")
    # for dep in conanfile.dependencies.values():
    for dep in conanfile.dependencies.filter({"direct": True}).values():
        if dep.package_folder is None:
            continue
        folder_name = os.path.join(dep.ref.name, str(dep.ref.version))
        build_type = dep.info.settings.get_safe("build_type")
        arch = dep.info.settings.get_safe("arch")
        if build_type:
            folder_name = os.path.join(folder_name, build_type)
        # if arch:
        #     folder_name = os.path.join(folder_name, arch)
        _deploy_single(dep, conanfile, output_folder, folder_name)
