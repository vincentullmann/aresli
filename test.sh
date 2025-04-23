



set install_dir=$(dirname $(readlink -f $0))/build/install

# Linux
# export PYTHONPATH=${REPO_ROOT}/dist/${RESOLVER_NAME}/lib/python:${PYTHONPATH}
export PXR_PLUGINPATH_NAME=${install_dir}/resources:${PXR_PLUGINPATH_NAME}
export LD_LIBRARY_PATH=${install_dir}
export TF_DEBUG=AR_RESOLVER_INIT # Debug Logs

usdview test.usda

