Taxonomy Menu Block - readme.txt 17/07/2012
-------------------

This module will enable you to display all your existing vocabularies in blocks. Enable the module and go to admin/config/development/taxonomy_menu_block to start adding your blocks.
You can add multiple blocks for each vocabulary and configure each one differently, all available settings are displayed on the add/edit page. 

This module works together with i18n. 

Do note that enabling the node-related options will cause a heavier server load, as it requires the module to run more database queries.

To take over the theming of your blocks, override the 'theme_taxonomy_menu_block' function. 