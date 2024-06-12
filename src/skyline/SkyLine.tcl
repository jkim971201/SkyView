proc read_lef { arg } {
  skyline::read_lef_cmd $arg
}

proc read_def { arg } {
  skyline::read_def_cmd $arg
}

proc read_verilog { arg } {
  skyline::read_verilog_cmd $arg
}

proc set_top_name { arg } {
  skyline::set_top_module_name_cmd $arg
}

proc display { } {
  skyline::display_cmd
}
