(function($) {
  Drupal.behaviors.Taxonomy_Menu_Block = {
    attach: function (context) {
      if ($("#edit-active-trail-ct").is(':checked')) {
        $("#edit-ct").show();
      } else {
        $("#edit-ct").hide();
      }
        
      $("#edit-active-trail-ct").click(function(){
        $('#edit-ct').toggle();
      });
    }
  };
})(jQuery);