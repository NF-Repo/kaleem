(function ($) {
  var WATERWAYS_AFFIX_OFFSET = 20;
  $(document).ready(function() {
    waterways_affix_sidebar('.region-sidebar-first-affix');
    waterways_affix_sidebar('.region-sidebar-second-affix');
  });
  function waterways_affix_sidebar(selector) {
    var sidebar = $(selector);
    if (sidebar.size()) {
      var offset = {
        top: sidebar.offset().top - WATERWAYS_AFFIX_OFFSET,
        bottom: $('#footer').outerHeight() + WATERWAYS_AFFIX_OFFSET
      }
      sidebar.width(sidebar.width());
      $('head').append('<style>' + selector + '.affix { top: ' + WATERWAYS_AFFIX_OFFSET + 'px; }</style>');
      $('head').append('<style>' + selector + '.affix-bottom { position: absolute; top: auto; bottom: ' + offset.bottom + 'px; }</style>');
      sidebar.affix({ offset: offset });
    }
  }
})(jQuery);
