<?php 

function bookpay_admin_overview() {
  module_load_include('admin.inc', 'pay', 'includes/pay');
  return pay_admin_pay_form_list('bookpay');
}
