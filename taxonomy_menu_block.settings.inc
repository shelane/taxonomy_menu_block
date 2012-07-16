<?php

/**
 * @file
 * Administrative page callbacks for the Taxonomy menu block module
 */

/**
 * Callback for page admin/config/development/taxonomy_menu_block/add
 */
function taxonomy_menu_block_form($form, $form_state) {

  // extract existing values if we are editing/cloning and set title of form
  if (isset($form_state['build_info']['args'][1])) {
    $tmb = $form_state['build_info']['args'][0];
    $title = t('Edit Taxonomy menu block - ') . $tmb['name'];
  } elseif (isset($form_state['build_info']['args'][0]['vid'])) {
    $tmb = $form_state['build_info']['args'][0];
    $title = t('Clone Taxonomy menu block');
  } else {
    $title = t('Add a new Taxonomy menu block');
  }

  // add js & pass on our array of node & field types to jquery
  drupal_add_js(drupal_get_path('module', 'taxonomy_menu_block') . '/taxonomy_menu_block.js');

  // get all taxonomy vocabularies
  $vocabs = array();
  foreach (taxonomy_vocabulary_get_names() as $vocab) {
    $vocabs[$vocab->vid] = $vocab->name;
  }

  $content_types = taxonomy_menu_block_ct_dropdown_options();
  $ct = isset($tmb['content_type']) ? $tmb['content_type'] : key($content_types);

  $form = array();
  // make form
  $form['title'] = array(
      '#type' => 'item',
      '#markup' => '<strong>' . strtoupper($title) . '</strong>',
  );
  $form['delta'] = array(
      '#type' => 'hidden',
      '#default_value' => !empty($form_state['build_info']['args'][1]) ? $form_state['build_info']['args'][1] : NULL,
  );
  $form['base'] = array(
      '#type' => 'fieldset',
      '#title' => 'Vocabulary',
      '#weight' => 1,
  );
  $form['base']['vid'] = array(
      '#type' => 'select',
      '#title' => t('Select a Vocabulary'),
      '#default_value' => !empty($tmb['vid']) ? $tmb['vid'] : '',
      '#options' => $vocabs,
  );
  $form['base']['name'] = array(
      '#type' => 'textfield',
      '#title' => t('Name'),
      '#required' => TRUE,
      '#maxlength' => 128,
      '#description' => t('Give your block a name to recognize it on the blocks admin page. Only alphanumeric characters and spaces are allowed. Must be unique.'),
      '#default_value' => !empty($tmb['name']) ? $tmb['name'] : '',
  );
  $form['levels'] = array(
      '#type' => 'fieldset',
      '#title' => 'Levels',
      '#description' => t('Counting starts from 1 for all the options below, i.e. 1 is the highest level in our taxonomy tree.'),
      '#weight' => 4,
  );
  $form['levels']['depth'] = array(
      '#type' => 'textfield',
      '#title' => t('Depth'),
      '#size' => 1,
      '#description' => t('Until which level should your taxonomy be rendered? Leave blank to display all levels.'),
      '#default_value' => isset($tmb['depth']) ? $tmb['depth'] : '',
  );
  $form['levels']['starting_level'] = array(
      '#type' => 'textfield',
      '#title' => t('Starting level'),
      '#size' => 1,
      '#description' => t('Should your taxonomy only be rendered from a certain level?'),
      '#default_value' => isset($tmb['starting_level']) ? $tmb['starting_level'] + 1 : '',
  );
  $form['levels']['level_as_text'] = array(
      '#type' => 'textfield',
      '#title' => t('Render level as just text'),
      '#size' => 1,
      '#description' => t('Would you like to render a level as text instead of links?'),
      '#default_value' => isset($tmb['level_as_text']) ? $tmb['level_as_text'] + 1 : '',
  );
  $form['node_options'] = array(
      '#type' => 'fieldset',
      '#title' => 'Node options',
      '#weight' => 7,
  );
  $form['node_options']['node_count'] = array(
      '#type' => 'checkbox',
      '#title' => t('Show node count'),
      '#description' => t('Would you like to show the number of nodes associated with this term?'),
      '#default_value' => isset($tmb['node_count']) ? $tmb['node_count'] : '',
  );
  $form['node_options']['orphan_terms'] = array(
      '#type' => 'checkbox',
      '#title' => t('Don\'t display terms with no nodes attched'),
      '#description' => t('Checking this option will leave out the terms that don\'t have any nodes attched to them. DOES NOT WORK YET!'),
      '#default_value' => isset($tmb['orphan_terms']) ? $tmb['orphan_terms'] : '',
  );
  $form['css'] = array(
      '#type' => 'fieldset',
      '#title' => 'CSS options',
      '#weight' => 8,
  );
  $form['css']['classes'] = array(
      '#type' => 'checkbox',
      '#title' => t('Add level-specific classes to list items?'),
      '#description' => t('Checking this will add a class specific to the depth of the taxonomy tree to your list items.'),
      '#default_value' => isset($tmb['classes']) ? $tmb['classes'] : '1',
  );
  $form['css']['extra_classes'] = array(
      '#type' => 'textfield',
      '#title' => t('Extra CSS classes'),
      '#description' => t('Extra classes you would like to add to your list items. Seperate classes by spaces.'),
      '#default_value' => isset($tmb['extra_classes']) ? $tmb['extra_classes'] : '',
  );
  $form['active_trail'] = array(
      '#type' => 'fieldset',
      '#title' => 'Active trail',
      '#weight' => 9,
  );
  $form['active_trail']['active_trail'] = array(
      '#type' => 'checkbox',
      '#title' => 'Follow active trail',
      '#description' => t('Add classes "active" and "active-trail" to your list items'),
      '#default_value' => isset($tmb['active_trail']) ? $tmb['active_trail'] : '1',
  );
  $form['active_trail']['active_trail_ct'] = array(
      '#type' => 'checkbox',
      '#title' => 'Keep active trail for a certain content type?',
      '#description' => t('Add classes "active" and "active-trail" to your list items when on a node page coupled with a term'),
      '#default_value' => isset($tmb['active_trail_ct']) ? $tmb['active_trail_ct'] : '0',
  );
  $form['active_trail']['ct'] = array(
      '#type' => 'fieldset',
  );
  $form['active_trail']['ct']['content_type'] = array(
      '#type' => 'select',
      '#title' => t('Select a content type'),
      '#description' => t('Select a content type'),
      '#options' => $content_types,
      '#default_value' => $ct,
      // Bind an ajax callback to the change event (which is the default for the
      // select form type) of the first dropdown. It will replace the second
      // dropdown when rebuilt
      '#ajax' => array(
          // When 'event' occurs, Drupal will perform an ajax request in the
          // background. Usually the default value is sufficient (eg. change for
          // select elements), but valid values include any jQuery event,
          // most notably 'mousedown', 'blur', and 'submit'.
          'event' => 'change',
          'callback' => 'taxonomy_menu_block_ajax_callback',
          'wrapper' => 'dropdown_term_reference',
      ),
  );
  $form['active_trail']['ct']['term_reference_field'] = array(
      '#type' => 'select',
      '#title' => t('Select a term reference field'),
      '#description' => t('Select a term ref'),
      // The entire enclosing div created here gets replaced when dropdown_first
      // is changed.
      '#prefix' => '<div id="dropdown_term_reference">',
      '#suffix' => '</div>',
      '#options' => taxonomy_menu_block_term_ref_options($ct),
      '#default_value' => isset($tmb['term_reference_field']) ? $tmb['term_reference_field'] : '',
  );

  $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Submit'),
      '#weight' => 10,
  );

  return $form;
}

/**
 * Callback for page admin/config/development/taxonomy_menu_block
 */
function taxonomy_menu_block_list() {
  $page = array();

  $page['intro'] = array(
      '#type' => 'item',
      '#markup' => t('The Taxonomy Menu Block module allows you to create blocks out of your existing taxonomy vocabularies. It works on multilingual sites.'),
  );

  $page['add_link'] = array(
      '#prefix' => '<ul class="action-links"><li>',
      '#markup' => l(t('Add a new Taxonomy menu block'), 'admin/config/development/taxonomy_menu_block/add'),
      '#suffix' => '</li></ul>',
  );

  // make our table header
  $header = array();
  $header[] = array('data' => t('Block'));
  $header[] = array('data' => t('Edit'));
  $header[] = array('data' => t('Delete'));
  $header[] = array('data' => t('Clone'));

  // grab our data
  $rows = array();
  $tmb_array = variable_get('taxonomy_menu_block', $default = NULL);
  if ($tmb_array != NULL) {
    $tmb_array = unserialize($tmb_array);
    foreach ($tmb_array as $delta => $tmb) {
      $row['data']['name'] = $tmb['name'];
      $row['data']['edit'] = l(t('edit'), 'admin/config/development/taxonomy_menu_block/edit/' . $delta);
      $row['data']['delete'] = l(t('delete'), 'admin/config/development/taxonomy_menu_block/delete/' . $delta);
      $row['data']['clone'] = l(t('clone'), 'admin/config/development/taxonomy_menu_block/clone/' . $delta);
      $rows[] = $row;
    }
  }

  // put together out table
  $page['content_table'] = array(
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $rows,
      '#empty' => t('No Taxonomy menu blocks available.'),
  );

  return $page;
}

/**
 * Callback for page admin/config/development/taxonomy_menu_block/edit/%
 */
function taxonomy_menu_block_form_edit() {
  // grab our data
  $tmb_array = variable_get('taxonomy_menu_block');
  $tmb_array = unserialize($tmb_array);

  // pass on our data to the form
  return drupal_get_form('taxonomy_menu_block_form', $tmb_array[arg(5)], arg(5));
}

/**
 * Callback for page admin/config/development/taxonomy_menu_block/clone/%
 */
function taxonomy_menu_block_form_clone() {
  // grab our data
  $tmb_array = variable_get('taxonomy_menu_block');
  $tmb_array = unserialize($tmb_array);

  drupal_set_message(t('You are now cloning Taxonomy Menu Block <i> ' . $tmb_array[arg(5)]['name'] . '</i>. Press submit again to save your new block.'), 'warning');

  //reset name
  $tmb_array[arg(5)]['name'] = NULL;

  // pass on our data to the form
  return drupal_get_form('taxonomy_menu_block_form', $tmb_array[arg(5)]);
}

/**
 * Callback for page admin/config/development/taxonomy_menu_block/delete/%
 */
function taxonomy_menu_block_form_delete() {
  $form = array();

  $form['delta'] = array(
      '#type' => 'hidden',
      '#default_value' => arg(5),
  );
  $form['submit'] = array(
      '#type' => 'submit',
      '#prefix' => t('Are you sure you want to delete this block?<br /><br />'),
      '#value' => t('Delete'),
  );

  return $form;
}

/**
 * Selects just the second dropdown to be returned for re-rendering
 *
 * Since the controlling logic for populating the form is in the form builder
 * function, all we do here is select the element and return it to be updated.
 *
 * @return renderable array (the second dropdown)
 */
function taxonomy_menu_block_ajax_callback($form, $form_state) {
   $form['active_trail']['ct']['term_reference_field'] = array(
      '#type' => 'select',
      '#title' => t('Select a term reference field'),
      '#description' => t('Select a term ref'),
      '#prefix' => '<div id="dropdown_term_reference">',
      '#suffix' => '</div>',
      '#options' => taxonomy_menu_block_term_ref_options($form_state["input"]["content_type"]),
  );
   
  return $form['active_trail']['ct']['term_reference_field'];
}

/**
 * Function to populate the first dropdown. 
 * @return array of options
 */
function taxonomy_menu_block_ct_dropdown_options() {
  $types = node_type_get_types();
  foreach ($types as $type) {
    if(count(taxonomy_menu_block_term_ref_options($type->type))>0){
    $node_types[$type->type] = $type->name;
    }
  }
  if (isset($node_types))
    return $node_types;
  else 
    return array();
}

/**
 * Function to populate the second dropdown. 
 * @param key. This will determine which set of options is returned.
 *
 * @return array of options
 */
function taxonomy_menu_block_term_ref_options($key = NULL) {
  if ($key != NULL) {
    $fields = field_info_instances('node', $key);
    foreach ($fields as $field) {
      $info = field_info_field($field['field_name']);
      if ($info['type'] == 'taxonomy_term_reference' || ($info['type'] == 'entityreference' && $info['settings']['target_type'] == 'taxonomy_term')){
        $node_fields[$key][$field['field_name']] = $field['label'];
      }
    }
    if (!empty($node_fields)) {
      return $node_fields[$key];
    } else {
      return array();
    }
    
  } else {
    return array();
  }
}