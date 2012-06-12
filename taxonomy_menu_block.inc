<?php
/**
 * @file
 * Administration settings form.
 */

/**
 * Callback for admin/config/taxonomy_menu_block;
 */ 
function taxonomy_menu_block_admin_settings() {
  $form = array();
  
  $vocabs = array();
  foreach(taxonomy_vocabulary_get_names() as $vocab) {
    $vocabs[$vocab->vid] = $vocab->name;
  }

  $form['taxonomy_menu_block_vid'] = array(
    '#type'           => 'select',
    '#title'          => t('Select a Vocabulary'),
    '#default_value'  => variable_get('taxonomy_menu_block_vid', 0),
    '#description'    => t('Select Vocabulary to be used by taxonomy menu block.'),
    '#options'        => $vocabs
  );  
  
  $form['taxonomy_menu_block_node_count'] = array(
    '#type'           => 'checkbox',
    '#title'          => t('Show node count'),
    '#default_value'  => variable_get('taxonomy_menu_block_node_count', 0)
  );    
  
  $form['taxonomy_menu_block_depth'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Depth?'),
    '#default_value'  => variable_get('taxonomy_menu_block_depth', 1)
  );
  
  return system_settings_form($form);
}