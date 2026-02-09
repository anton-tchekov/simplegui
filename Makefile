# Make all examples
all:
	clear
	cd examples/basic-app/ && make $@
	cd examples/change-font/ && make $@
	cd examples/simple-button/ && make $@
	cd examples/simple-checkbox/ && make $@
	cd examples/simple-select/ && make $@
	cd examples/simple-slider/ && make $@
	cd examples/simple-textbox/ && make $@
	cd examples/styling/ && make $@
	cd examples/power-usage-calc/ && make $@
	cd examples/controller/ && make $@
