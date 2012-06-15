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

  // extract existing values if we are editing and set title of form
  if ($args['build_info']['args'] != NULL) {
    $tmb = $args['build_info']['args'][0];
    $title = t('Edit a Taxonomy menu block');
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
  );
  $form['taxonomy_menu_block']['vid'] = array(
      '#type' => 'select',
      '#title' => t('Select a Vocabulary'),
      '#description' => t('Select a Vocabulary for your new block'),
      '#default_value' => !empty($tmb['vid']) ? $tmb['vid'] : '',
      '#options' => $vocabs,
  );
  $form['taxonomy_menu_block']['name'] = array(
      '#type' => 'textfield',
      '#title' => t('Name'),
      '#required' => TRUE,
      '#maxlength' => 128,
      '#description' => t('Give your block a name to recognize it on the blocks admin page. Only alphanumeric characters and spaces are allowed. Must be unique.'),
      '#default_value' => !empty($tmb['name']) ? $tmb['name'] : '',
  );
  $form['taxonomy_menu_block']['depth'] = array(
      '#type' => 'textfield',
      '#title' => t('Depth'),
      '#size' => 1,
      '#description' => t('Until which level should your taxonomy be rendered? Leave blank to display all levels.'),
      '#default_value' => isset($tmb['depth']) ? $tmb['depth'] : '',
  );
  $form['taxonomy_menu_block']['level_as_text'] = array(
      '#type' => 'textfield',
      '#title' => t('Render level as just text'),
      '#size' => 1,
      '#description' => t('If you have a level you would like to render just a text instead of links, enter it here. The highest level is zero.'),
      '#default_value' => isset($tmb['level_as_text']) ? $tmb['level_as_text'] : '',
  );
  $form['taxonomy_menu_block']['node_count'] = array(
      '#type' => 'checkbox',
      '#title' => t('Show node count'),
      '#description' => t('Would you like to show the number of nodes associated with this term?'),
      '#default_value' => !empty($tmb['node_count']) ? $tmb['node_count'] : '',
  );
  $form['taxonomy_menu_block']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Submit'),
  );

  return $form;
}

/**
 * Callback for page admin/config/development/taxonomy_menu_block
 */
function taxonomy_menu_block_list() {
  $page = array();

  $page['add_link'] = array(
      '#type' => 'link',
      '#title' => t('Add a new Taxonomy menu block'),
      '#href' => 'admin/config/development/taxonomy_menu_block/add',
      '#suffix' => '<br /><br />',
  );

  // make our table header
  $header = array();
  $header[] = array('data' => t('Block'));
  $header[] = array('data' => t('Edit'));
  $header[] = array('data' => t('Delete'));

  // grab our data
  $rows = array();
  $tmb_array = variable_get('taxonomy_menu_block', $default = NULL);
  if ($tmb_array != NULL) {
    $tmb_array = unserialize($tmb_array);
    foreach ($tmb_array as $delta => $tmb) {
      $row['data']['name'] = $tmb['name'];
      $row['data']['edit'] = l(t('edit'), 'admin/config/development/taxonomy_menu_block/edit/' . $delta);
      $row['data']['delete'] = l(t('delete'), 'admin/config/development/taxonomy_menu_block/delete/' . $delta);
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