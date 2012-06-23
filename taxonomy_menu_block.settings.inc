<?php

/**
 * @file
 * Administrative page callbacks for the Taxonomy menu block module
 */

/**
 * Callback for page admin/config/development/taxonomy_menu_block/add
 */
function taxonomy_menu_block_form($form_state, $args = NULL) {
  $form = array();

  // get all taxonomy vocabularies
  $vocabs = array();
  foreach (taxonomy_vocabulary_get_names() as $vocab) {
    $vocabs[$vocab->vid] = $vocab->name;
  }

  // extract existing values if we are editing/cloning and set title of form
  if (isset($args['build_info']['args'][1])) {
    $tmb = $args['build_info']['args'][0];
    $title = t('Edit Taxonomy menu block - ') . $tmb['name'];
  } elseif (isset($args['build_info']['args'][0]['vid'])) {
    $tmb = $args['build_info']['args'][0];
    $title = t('Clone Taxonomy menu block');
  } else {
    $title = t('Add a new Taxonomy menu block');
  }

  // make form
  $form['taxonomy_menu_block'] = array(
      '#type' => 'fieldset',
      '#title' => $title,
  );
  $form['taxonomy_menu_block']['delta'] = array(
      '#type' => 'hidden',
      '#default_value' => !empty($args['build_info']['args'][1]) ? $args['build_info']['args'][1] : NULL,
      '#weight' => 1,
  );
  $form['taxonomy_menu_block']['vid'] = array(
      '#type' => 'select',
      '#title' => t('Select a Vocabulary'),
      '#description' => t('Select a Vocabulary for your new block'),
      '#default_value' => !empty($tmb['vid']) ? $tmb['vid'] : '',
      '#options' => $vocabs,
      '#weight' => 2,
  );
  $form['taxonomy_menu_block']['name'] = array(
      '#type' => 'textfield',
      '#title' => t('Name'),
      '#required' => TRUE,
      '#maxlength' => 128,
      '#description' => t('Give your block a name to recognize it on the blocks admin page. Only alphanumeric characters and spaces are allowed. Must be unique.'),
      '#default_value' => !empty($tmb['name']) ? $tmb['name'] : '',
      '#weight' => 3,
  );
  $form['taxonomy_menu_block']['depth'] = array(
      '#type' => 'textfield',
      '#title' => t('Depth'),
      '#size' => 1,
      '#description' => t('Until which level should your taxonomy be rendered? Leave blank to display all levels.'),
      '#default_value' => isset($tmb['depth']) ? $tmb['depth'] : '',
      '#weight' => 4
  );
  $form['taxonomy_menu_block']['starting_level'] = array(
      '#type' => 'textfield',
      '#title' => t('Starting level'),
      '#size' => 1,
      '#description' => t('Should your taxonomy only be rendered from a certain level?'),
      '#default_value' => isset($tmb['starting_level']) ? $tmb['starting_level'] + 1 : '',
      '#weight' => 5,
  );
  $form['taxonomy_menu_block']['level_as_text'] = array(
      '#type' => 'textfield',
      '#title' => t('Render level as just text'),
      '#size' => 1,
      '#description' => t('If you have a level you would like to render just a text instead of links, enter it here.'),
      '#default_value' => isset($tmb['level_as_text']) ? $tmb['level_as_text'] + 1 : '',
      '#weight' => 6,
  );
  $form['taxonomy_menu_block']['node_count'] = array(
      '#type' => 'checkbox',
      '#title' => t('Show node count'),
      '#description' => t('Would you like to show the number of nodes associated with this term?'),
      '#default_value' => isset($tmb['node_count']) ? $tmb['node_count'] : '',
      '#weight' => 7,
  );
  $form['taxonomy_menu_block']['css'] = array(
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#title' => 'CSS options',
      '#weight' => 8,
  );
  $form['taxonomy_menu_block']['css']['active_trail'] = array(
      '#type' => 'checkbox',
      '#title' => 'Follow active trail',
      '#description' => t('Add classes "active" and "active-trail" to your list items'),
      '#default_value' => isset($tmb['active_trail']) ? $tmb['active_trail'] : '1',
  );
  $form['taxonomy_menu_block']['css']['extra_classes'] = array(
      '#type' => 'textfield',
      '#title' => t('Extra CSS classes'),
      '#description' => t('Extra classes you would like to add to your list items. Seperate classes by spaces.'),
      '#default_value' => isset($tmb['extra_classes']) ? $tmb['extra_classes'] : '',
  );
  $form['taxonomy_menu_block']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Submit'),
      '#weight' => 9,
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

  $form['taxonomy_menu_block']['delta'] = array(
      '#type' => 'hidden',
      '#default_value' => arg(5),
  );
  $form['taxonomy_menu_block']['submit'] = array(
      '#type' => 'submit',
      '#prefix' => t('Are you sure you want to delete this block?<br /><br />'),
      '#value' => t('Delete'),
  );

  return $form;
}