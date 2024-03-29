<?php

/**
 * @file
 * Holds functions for the paragraphs widgets.
 */

/**
 * Implements hook_field_widget_info().
 */
function paragraphs_field_widget_info() {
  return array(
    'paragraphs_hidden' => array(
      'label' => t('Hidden'),
      'field types' => array('paragraphs'),
      'behaviors' => array(
        'multiple values' => FIELD_BEHAVIOR_CUSTOM,
        'default value' => FIELD_BEHAVIOR_NONE,
      ),
    ),
    'paragraphs_embed' => array(
      'label' => t('Embedded'),
      'field types' => array('paragraphs'),
      'behaviors' => array(
        'multiple values' => FIELD_BEHAVIOR_CUSTOM,
        'default value' => FIELD_BEHAVIOR_NONE,
      ),
    ),
  );
}

/**
 * Implements hook_field_widget_form().
 */
function paragraphs_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {
  switch ($instance['widget']['type']) {
    case 'paragraphs_hidden':
      return $element;

      break;
    case 'paragraphs_embed':
      $form['#entity'] = $element['#entity'];
      return paragraphs_field_multiple_value_form($field, $instance, $langcode, $items, $form, $form_state);
      break;
  }
}

/**
 * Special handling to create form elements for multiple values.
 *
 * Handles generic features for multiple fields:
 * - number of widgets
 * - AHAH-'add more' button
 * - drag-n-drop value reordering
 */
function paragraphs_field_multiple_value_form($field, $instance, $langcode, $items, &$form, &$form_state) {
  $field_name = $field['field_name'];
  $parents = $form['#parents'];

  $field_state = field_form_get_state($parents, $field_name, $langcode, $form_state);
  $max = $field_state['items_count'] - 1;

  $load_entities = TRUE;
  if (isset($field_state['removed_one']) && $field_state['removed_one']) {
    $load_entities = FALSE;
  }

  $title = check_plain($instance['label']);
  $description = field_filter_xss($instance['description']);

  $id_prefix = implode('-', array_merge($parents, array($field_name)));
  $wrapper_id = drupal_html_id($id_prefix . '-add-more-wrapper');

  $field_elements = array();

  $function = $instance['widget']['module'] . '_field_widget_form_build';
  if (function_exists($function)) {
    for ($delta = 0; $delta <= $max; $delta++) {
      $multiple = TRUE;
      $element = array(
        '#entity_type' => $instance['entity_type'],
        '#entity' => $form['#entity'],
        '#bundle' => $instance['bundle'],
        '#field_name' => $field_name,
        '#language' => $langcode,
        '#field_parents' => $parents,
        '#columns' => array_keys($field['columns']),
        // For multiple fields, title and description are handled by the wrapping table.
        '#title' => $multiple ? '' : $title,
        '#description' => $multiple ? '' : $description,
        // Only the first widget should be required.
        '#required' => $delta == 0 && $instance['required'],
        '#delta' => $delta,
        '#weight' => $delta,
      );
      if ($element = $function($form, $form_state, $field, $instance, $langcode, $items, $delta, $element, $load_entities)) {
        // Input field for the delta (drag-n-drop reordering).
        if ($multiple) {
          // We name the element '_weight' to avoid clashing with elements
          // defined by widget.
          $element['_weight'] = array(
            '#type' => 'weight',
            '#title' => t('Weight for row @number', array('@number' => $delta + 1)),
            '#title_display' => 'invisible',
            // Note: this 'delta' is the FAPI 'weight' element's property.
            '#delta' => $max,
            '#default_value' => isset($items[$delta]['_weight']) ? $items[$delta]['_weight'] : $delta,
            '#weight' => 100,
          );
        }

        // Allow modules to alter the field widget form element.
        $context = array(
          'form' => $form,
          'field' => $field,
          'instance' => $instance,
          'langcode' => $langcode,
          'items' => $items,
          'delta' => $delta,
        );
        drupal_alter(array('paragraphs_field_widget_form', 'paragraphs_field_widget_' . $instance['widget']['type'] . '_form'), $element, $form_state, $context);

        $field_elements[$delta] = $element;
      }
    }

    $field_elements += array(
      '#theme' => 'paragraphs_field_multiple_value_form',
      '#field_name' => $field['field_name'],
      '#cardinality' => $field['cardinality'],
      '#title' => $title,
      '#required' => $instance['required'],
      '#description' => $description,
      '#prefix' => '<div id="' . $wrapper_id . '">',
      '#suffix' => '</div>',
      '#max_delta' => $max,
      '#instance' => $instance,
    );

    // Add 'add more' button, if not working with a programmed form.
    if (empty($form_state['programmed'])) {
      $available_bundles = paragraphs_bundle_load();
      $select_bundles = array();
      $select_bundles_weighted = array();

      // By default, consider that no bundle has been explicitly picked.
      $explicitly_enabled = FALSE;
      foreach ($instance['settings']['allowed_bundles'] as $allowed_bundle_key => $allowed_bundle_value) {
        if ($allowed_bundle_key === $allowed_bundle_value && isset($available_bundles[$allowed_bundle_key])) {
          $select_bundles[$available_bundles[$allowed_bundle_key]->bundle] = $available_bundles[$allowed_bundle_key]->name;

          // If an item has been explicitly selected, raise our flag.
          $explicitly_enabled = TRUE;
        }
        @$select_bundles_weighted[$available_bundles[$allowed_bundle_key]->bundle] = $available_bundles[$allowed_bundle_key]->name;
      }

      // If no bundle has been explicitly selected, give access to all of them.
      if (!$explicitly_enabled) {
        $select_bundles = $select_bundles_weighted;
        foreach ($available_bundles as $bundle) {
          if (!isset($select_bundles[$bundle->bundle])) {
            $select_bundles[$bundle->bundle] = $bundle->name;
          }
        }
      }

      $removed_a_bundle = FALSE;

      foreach ($select_bundles as $machine_name => $bundle) {
        if (!entity_access('create', 'paragraphs_item', $machine_name)) {
          unset($select_bundles[$machine_name]);
          $removed_a_bundle = TRUE;
        }
      }

      if($removed_a_bundle && count($select_bundles) === 0) {
        $field_elements['add_more'] = array(
          '#type' => 'container',
          '#tree' => TRUE,
        );
        $field_elements['add_more']['add_more'] = array(
          '#type' => 'markup',
          '#markup' => '<em>' . t('You are not allowed to add any of the bundles.') . '</em>',
        );
      }
      elseif (count($select_bundles)) {
        $field = $field_state['field'];

        if ($field['cardinality'] == FIELD_CARDINALITY_UNLIMITED || $field_state['items_count'] < $field['cardinality']) {
          if (!isset($instance['settings']['title'])) {
            $instance['settings']['title'] = PARAGRAPHS_DEFAULT_TITLE;
          }

          $field_elements['add_more'] = array(
            '#type' => 'container',
            '#tree' => TRUE,
          );

          $add_mode = (isset($instance['settings']['add_mode']) ? $instance['settings']['add_mode'] : PARAGRAPHS_DEFAULT_ADD_MODE);

          if ($add_mode == 'button') {
            foreach ($select_bundles as $machine_name => $bundle_name) {
              $field_elements['add_more']['add_more_bundle_' . $machine_name] = array(
                '#type' => 'submit',
                '#name' => strtr($id_prefix, '-', '_') . '_add_more_add_more_bundle_' . $machine_name,
                '#value' => t('Add a !title paragraph', array('!title' => $bundle_name)),
                '#access' => entity_access('create', 'paragraphs_item', $machine_name),
                '#attributes' => array('class' => array('field-add-more-submit', 'paragraphs-add-more-submit')),
                '#limit_validation_errors' => array(),
                '#submit' => array('paragraphs_add_more_submit'),
                '#ajax' => array(
                  'callback' => 'paragraphs_add_more_js',
                  'wrapper' => $wrapper_id,
                  'effect' => 'fade',
                ),
              );
            }
          }
          else {
            $field_elements['add_more']['type'] = array(
              '#type' => 'select',
              '#name' => strtr($id_prefix, '-', '_') . '_add_more_type',
              '#title' => t('!title type', array('!title' => t($instance['settings']['title']))),
              '#options' => $select_bundles,
              '#attributes' => array('class' => array('field-add-more-type')),
              '#limit_validation_errors' => array(array_merge($parents, array($field_name, $langcode))),
            );

            // Hide the bundle selection if only one bundle is allowed.
            if (count($select_bundles) == 1) {
              $field_elements['add_more']['type']['#type'] = 'hidden';
              $keys = array_keys($select_bundles);
              $field_elements['add_more']['type']['#value'] = $keys[0];
            }

            if (isset($form_state['input'][strtr($id_prefix, '-', '_') . '_add_more_type'])) {
              $field_elements['add_more']['type']['#default_value'] = $form_state['input'][strtr($id_prefix, '-', '_') . '_add_more_type'];
            }

            $text = 'Add a !title';
            if ($max >= 0) {
              $text = 'Add another !title';
            }

            $field_elements['add_more']['add_more'] = array(
              '#type' => 'submit',
              '#name' => strtr($id_prefix, '-', '_') . '_add_more_add_more',
              '#value' => t($text, array('!title' => t($instance['settings']['title']))),
              '#attributes' => array('class' => array('field-add-more-submit', 'paragraphs-add-more-submit')),
              '#limit_validation_errors' => array(),
              '#submit' => array('paragraphs_add_more_submit'),
              '#ajax' => array(
                'callback' => 'paragraphs_add_more_js',
                'wrapper' => $wrapper_id,
                'effect' => 'fade',
              ),
            );
          }
        }
      }
      else {
        $field_elements['add_more']['add_more'] = array(
          '#type' => 'markup',
          '#markup' => '<em>' . t('No bundles available, edit field settings') . '</em>',
        );
      }
    }
  }

  if (module_exists('file')) {
    // file.js triggers uploads when the main Submit button is clicked.
    $field_elements['#attached']['js'] = array(
      drupal_get_path('module', 'file') . '/file.js',
      array('data' => drupal_get_path('module', 'paragraphs') . '/paragraphs.js', 'type' => 'file', 'weight' => 9999),
    );
    $form_state['has_file_element'] = TRUE;
  }

  return $field_elements;
}

/**
 * Widget form implementation for paragraphs.
 *
 * @param $form
 * @param $form_state
 * @param $field
 * @param $instance
 * @param $langcode
 * @param $items
 * @param $delta
 * @param $element
 * @param bool $load_entities
 *
 * @return array
 */
function paragraphs_field_widget_form_build(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element, $load_entities = TRUE) {
  static $recursion = 0;

  if (!isset($instance['settings']['title'])) {
    $instance['settings']['title'] = PARAGRAPHS_DEFAULT_TITLE;
  }

  if (!isset($instance['settings']['title_multiple'])) {
    $instance['settings']['title_multiple'] = PARAGRAPHS_DEFAULT_TITLE_MULTIPLE;
  }

  // If the paragraph item form contains another paragraph,
  // we might ran into a recursive loop. Prevent that.
  if ($recursion++ > 3) {
    drupal_set_message(t('The paragraphs item form has not been embedded to avoid recursive loops.'), 'error');
    return $element;
  }

  $field_parents = $element['#field_parents'];
  $field_name = $element['#field_name'];
  $language = $element['#language'];

  $bundle = FALSE;
  $id_prefix = implode('-', array_merge($field_parents, array($field_name)));

  if (isset($form_state['input'][strtr($id_prefix, '-', '_') . '_add_more_type'])) {
    $bundle = $form_state['input'][strtr($id_prefix, '-', '_') . '_add_more_type'];
  }
  elseif (isset($form_state['input']['_triggering_element_name'])) {
    if (strpos($form_state['input']['_triggering_element_name'], strtr($id_prefix, '-', '_') . '_add_more_add_more_bundle_') === 0) {
      $bundle = substr($form_state['input']['_triggering_element_name'], drupal_strlen(strtr($id_prefix, '-', '_') . '_add_more_add_more_bundle_'));
    }
  }

  // Nest the paragraphs item entity form in a dedicated parent space,
  // by appending [field_name, langcode, delta] to the current parent space.
  // That way the form values of the paragraphs item are separated.
  $parents = array_merge($field_parents, array($field_name, $language, $delta));

  $element += array(
    '#element_validate' => array('paragraphs_field_widget_embed_validate'),
    '#parents' => $parents,
  );

  $field_state = field_form_get_state($field_parents, $field_name, $language, $form_state);

  $deleted_paragraph = FALSE;
  $is_new_paragraph = FALSE;

  $default_edit_mode = isset($instance['settings']['default_edit_mode']) ? $instance['settings']['default_edit_mode'] : PARAGRAPHS_DEFAULT_EDIT_MODE;

  $being_edited_paragraph = TRUE;
  if ($default_edit_mode === 'closed' || $default_edit_mode === 'preview') {
    $being_edited_paragraph = FALSE;
  }

  if (isset($field_state['entity'][$delta])) {
    if (isset($field_state['entity'][$delta]->removed) && $field_state['entity'][$delta]->removed) {
      $deleted_paragraph = TRUE;
    }
    if ($being_edited_paragraph || (isset($field_state['entity'][$delta]->being_edited) && $field_state['entity'][$delta]->being_edited)) {
      $being_edited_paragraph = TRUE;
    }
    else {
      $being_edited_paragraph = FALSE;
    }
    $paragraph_item = $field_state['entity'][$delta];
  }
  else {
    if (isset($items[$delta]) && $load_entities) {
      $paragraph_item = paragraphs_field_get_entity($items[$delta]);
    }

    // Show an empty collection if we have no existing one or it does not
    // load.
    if (empty($paragraph_item) && $bundle) {
      $paragraph_item = entity_create('paragraphs_item', array('bundle' => $bundle, 'field_name' => $field_name));
      $paragraph_item->being_edited = TRUE;
      $being_edited_paragraph = TRUE;
      $is_new_paragraph = TRUE;
    }

    if (!empty($paragraph_item)) {
      // Put our entity in the form state, so FAPI callbacks can access it.
      $field_state['entity'][$delta] = $paragraph_item;
    }
  }
  field_form_set_state($field_parents, $field_name, $language, $form_state, $field_state);
  if (!empty($paragraph_item)) {

    $bundle_info = paragraphs_bundle_load($paragraph_item->bundle);

    if ($bundle_info) {
      $element['paragraph_bundle_title'] = array(
        '#type' => 'container',
        '#weight' => -100,
      );
      $element['paragraph_bundle_title']['info'] = array(
        '#markup' => t('!title type: %bundle', array('!title' => t($instance['settings']['title']), '%bundle' => $bundle_info->name)),
      );
    }

    if (!$deleted_paragraph) {
      $element['actions'] = array(
        '#type' => 'actions',
        '#weight' => 9999,
      );

      field_attach_form('paragraphs_item', $paragraph_item, $element, $form_state, $language);
      if ($being_edited_paragraph) {
        if (!$is_new_paragraph && !entity_access('update', 'paragraphs_item', $paragraph_item)) {
          foreach (element_children($element) as $key) {
            if ($key != 'paragraph_bundle_title' && $key != 'actions' && $key != 'paragraph_bundle_preview' && $key != 'access_info') {
              $element[$key]['#access'] = FALSE;
            }
          }

          $element['access_info'] = array(
            '#type' => 'container',
            '#weight' => 9998,
          );
          $element['access_info']['info'] = array(
            '#type' => 'markup',
            '#markup' => '<em>' . t('You are not allowed to edit this !title item.', array('!title' => t($instance['settings']['title']))) . '</em>',
          );
        }
        else {
          if (empty($element['#required'])) {
            $element['#after_build'][] = 'paragraphs_field_widget_embed_delay_required_validation';
          }
        }
      }
      else {
        if($default_edit_mode === 'preview' && entity_access('view', 'paragraphs_item', $paragraph_item)) {
          $element['paragraph_bundle_preview'] = array(
            '#type' => 'container',
          );
          $preview = $paragraph_item->view('paragraphs_editor_preview');
          $element['paragraph_bundle_preview']['preview'] = $preview;
        }

        foreach (element_children($element) as $key) {
          if ($key != 'paragraph_bundle_title' && $key != 'actions' && $key != 'paragraph_bundle_preview' && $key != 'access_info') {
            $element[$key]['#access'] = FALSE;
          }
        }
        $element['actions'] = array(
          '#type' => 'actions',
          '#weight' => 9999,
        );
        $element['actions']['edit_button'] = array(
          '#delta' => $delta,
          '#name' => implode('_', $parents) . '_edit_button',
          '#type' => 'submit',
          '#value' => t('Edit !type paragraph', array('!type' => $bundle_info->name)),
          '#validate' => array(),
          '#submit' => array('paragraphs_edit_submit'),
          '#limit_validation_errors' => array(),
          '#ajax' => array(
            'path' => 'paragraphs/edit/ajax',
            'effect' => 'fade',
          ),
          '#access' => entity_access('update', 'paragraphs_item', $paragraph_item),
          '#weight' => 999,
        );
      }


      if (isset($paragraph_item)) {
        $element['actions']['remove_button'] = array(
          '#delta' => $delta,
          '#name' => implode('_', $parents) . '_remove_button',
          '#type' => 'submit',
          '#value' => t('Remove !type paragraph', array('!type' => $bundle_info->name)),
          '#validate' => array(),
          '#submit' => array('paragraphs_remove_submit'),
          '#limit_validation_errors' => array(),
          '#ajax' => array(
            'path' => 'paragraphs/remove/ajax',
            'effect' => 'fade',
          ),
          '#access' => entity_access('delete', 'paragraphs_item', $paragraph_item),
          '#weight' => 1000,
        );
      }


      if (isset($element['actions']['edit_button']) && !$element['actions']['edit_button']['#access']
          && isset($element['actions']['remove_button']) && !$element['actions']['remove_button']['#access']) {
        $element['access_info'] = array(
          '#type' => 'container',
          '#weight' => 9998,
        );
        $element['access_info']['info'] = array(
          '#type' => 'markup',
          '#markup' => '<em>' . t('You are not allowed to edit or remove this !title item.', array('!title' => t($instance['settings']['title']))) . '</em>',
        );
      }
      elseif (isset($element['actions']['edit_button']) && !$element['actions']['edit_button']['#access']) {
        $element['access_info'] = array(
          '#type' => 'container',
          '#weight' => 9998,
        );
        $element['access_info']['info'] = array(
          '#type' => 'markup',
          '#markup' => '<em>' . t('You are not allowed to edit this !title item.', array('!title' => t($instance['settings']['title']))) . '</em>',
        );
      }
      elseif (isset($element['actions']['remove_button']) && !$element['actions']['remove_button']['#access']) {
        $element['access_info'] = array(
          '#type' => 'container',
          '#weight' => 9998,
        );
        $element['access_info']['info'] = array(
          '#type' => 'markup',
          '#markup' => '<em>' . t('You are not allowed to remove this !title item.', array('!title' => t($instance['settings']['title']))) . '</em>',
        );
      }
    }
    else {
      $element['actions'] = array(
        '#type' => 'actions',
        '#weight' => 9999,
      );
      $element['actions']['remove_button'] = array(
        '#markup' => '<p>' . t('This !title has been removed, press the button below to restore.', array('!title' => t($instance['settings']['title']))) . ' </p><p><em>' . t('Warning: this !title will actually be deleted when you press "!confirm" or "!save"!', array('!title' => $instance['settings']['title'], '!confirm' => t('Confirm Deletion'), '!save' => t('Save'))) . '</em></p>',
      );
      $element['actions']['restore_button'] = array(
        '#delta' => $delta,
        '#name' => implode('_', $parents) . '_restore_button',
        '#type' => 'submit',
        '#value' => t('Restore'),
        '#validate' => array(),
        '#submit' => array('paragraphs_restore_submit'),
        '#limit_validation_errors' => array(),
        '#ajax' => array(
          'path' => 'paragraphs/restore/ajax',
          'effect' => 'fade',
        ),
        '#weight' => 1000,
      );
      $element['actions']['confirm_delete_button'] = array(
        '#delta' => $delta,
        '#name' => implode('_', $parents) . '_deleteconfirm_button',
        '#type' => 'submit',
        '#value' => t('Confirm Deletion'),
        '#validate' => array(),
        '#submit' => array('paragraphs_deleteconfirm_submit'),
        '#limit_validation_errors' => array(),
        '#ajax' => array(
          'path' => 'paragraphs/deleteconfirm/ajax',
          'effect' => 'fade',
        ),
        '#weight' => 1001,
      );
    }
  }

  $recursion--;
  return $element;
}


/**
 * FAPI #after_build of an individual paragraph element to delay the validation of #required.
 */
function paragraphs_field_widget_embed_delay_required_validation(&$element, &$form_state) {
  // If the process_input flag is set, the form and its input is going to be
  // validated. Prevent #required (sub)fields from throwing errors while
  // their non-#required paragraph item is empty.
  if ($form_state['process_input']) {
    _paragraphs_collect_required_elements($element, $element['#paragraphs_required_elements']);
  }
  return $element;
}

/**
 * Collects all embedded required fields.
 *
 * @param $element
 * @param $required_elements
 */
function _paragraphs_collect_required_elements(&$element, &$required_elements) {
  // Recurse through all children.
  foreach (element_children($element) as $key) {
    if (isset($element[$key]) && $element[$key]) {
      _paragraphs_collect_required_elements($element[$key], $required_elements);
    }
  }
  if (!empty($element['#required'])) {
    $required_elements[] = &$element;
    $element += array('#pre_render' => array());
    array_unshift($element['#pre_render'], 'paragraphs_field_widget_render_required');
  }
}

/**
 * #pre_render callback that ensures the element is rendered as being required.
 */
function paragraphs_field_widget_render_required($element) {
  $element['#required'] = TRUE;
  return $element;
}

/**
 * FAPI validation of an individual paragraph element.
 */
function paragraphs_field_widget_embed_validate($element, &$form_state, $complete_form) {
  $instance = field_widget_instance($element, $form_state);
  $field = field_widget_field($element, $form_state);
  $field_parents = $element['#field_parents'];
  $field_name = $element['#field_name'];
  $language = $element['#language'];

  $field_state = field_form_get_state($field_parents, $field_name, $language, $form_state);
  if (isset($field_state['entity'][$element['#delta']])) {
    $paragraph_item = $field_state['entity'][$element['#delta']];

    // Attach field API validation of the embedded form.
    field_attach_form_validate('paragraphs_item', $paragraph_item, $element, $form_state);

    // Now validate required elements if the entity is not empty.
    if ((!isset($paragraph_item->removed) || !$paragraph_item->removed) && !empty($element['#paragraphs_required_elements'])) {
      foreach ($element['#paragraphs_required_elements'] as &$elements) {

        // Copied from _form_validate().
        if (isset($elements['#needs_validation'])) {
          $is_empty_multiple = (!count($elements['#value']));
          $is_empty_string = (is_string($elements['#value']) && drupal_strlen(trim($elements['#value'])) == 0);
          $is_empty_value = ($elements['#value'] === 0);
          if ($is_empty_multiple || $is_empty_string || $is_empty_value) {
            if (isset($elements['#title'])) {
              $error_text = t('!name field is required.', array('!name' => $elements['#title']));
              form_error($elements, filter_xss_admin($error_text));
            }
            else {
              form_error($elements);
            }
          }
        }
      }
    }

    // Only if the form is being submitted, finish the collection entity and
    // prepare it for saving.
    if ($form_state['submitted'] && !form_get_errors()) {

      field_attach_submit('paragraphs_item', $paragraph_item, $element, $form_state);

      // Load initial form values into $item, so any other form values below the
      // same parents are kept.
      $item = drupal_array_get_nested_value($form_state['values'], $element['#parents']);

      // Set the _weight if it is a multiple field.
      if (isset($element['_weight'])) {
        $item['_weight'] = $element['_weight']['#value'];
      }

      // Put the paragraph item in $item['entity'], so it is saved with
      // the host entity via hook_field_presave() / field API if it is not empty.
      // @see paragraph_field_presave()
      $item['entity'] = $paragraph_item;
      form_set_value($element, $item, $form_state);
    }
  }
}

/**
 * Submit function to add another paragraph.
 * @param $form
 * @param $form_state
 */
function paragraphs_add_more_submit($form, &$form_state) {
  $button = $form_state['triggering_element'];

  // Go one level up in the form, to the widgets container.
  $element = drupal_array_get_nested_value($form, array_slice($button['#array_parents'], 0, -2));
  $field_name = $element['#field_name'];
  $langcode = $element['#language'];
  $parents = $element['#field_parents'];

  // Increment the items count.
  $field_state = field_form_get_state($parents, $field_name, $langcode, $form_state);

  $field = $field_state['field'];

  if ($field['cardinality'] == FIELD_CARDINALITY_UNLIMITED || $field_state['items_count'] < $field['cardinality']) {
    $field_state['items_count']++;
  }

  field_form_set_state($parents, $field_name, $langcode, $form_state, $field_state);

  $form_state['rebuild'] = TRUE;
}

/**
 * Ajax callback in response to a new empty widget being added to the form.
 *
 * This returns the new page content to replace the page content made obsolete
 * by the form submission.
 *
 * @see field_add_more_submit()
 */
function paragraphs_add_more_js($form, $form_state) {
  $button = $form_state['triggering_element'];

  // Go one level up in the form, to the widgets container.
  $element = drupal_array_get_nested_value($form, array_slice($button['#array_parents'], 0, -2));
  $field_name = $element['#field_name'];
  $langcode = $element['#language'];
  $parents = $element['#field_parents'];

  $field_state = field_form_get_state($parents, $field_name, $langcode, $form_state);

  $field = $field_state['field'];

  // Add a DIV around the delta receiving the Ajax effect.
  $delta = $element['#max_delta'];
  $element[$delta]['#prefix'] = '<div class="ajax-new-content">' . (isset($element[$delta]['#prefix']) ? $element[$delta]['#prefix'] : '');
  $element[$delta]['#suffix'] = (isset($element[$delta]['#suffix']) ? $element[$delta]['#suffix'] : '') . '</div>';

  return $element;
}


/**
 * Submit callback to remove an item from the field UI multiple wrapper.
 *
 * When a remove button is submitted, we need to find the item that it
 * referenced and delete it. Since field UI has the deltas as a straight
 * unbroken array key, we have to renumber everything down. Since we do this
 * we *also* need to move all the deltas around in the $form_state['values']
 * and $form_state['input'] so that user changed values follow. This is a bit
 * of a complicated process.
 */
function paragraphs_remove_submit($form, &$form_state) {
  $button = $form_state['triggering_element'];
  $delta = $button['#delta'];

  // Where in the form we'll find the parent element.
  $address = array_slice($button['#array_parents'], 0, -3);

  // Go one level up in the form, to the widgets container.
  $parent_element = drupal_array_get_nested_value($form, $address);
  $field_name = $parent_element['#field_name'];
  $langcode = $parent_element['#language'];
  $parents = $parent_element['#field_parents'];

  $field_state = field_form_get_state($parents, $field_name, $langcode, $form_state);

  if (isset($field_state['entity'][$delta])) {
    $field_state['entity'][$delta]->removed = 1;
  }

  // Fix the weights. Field UI lets the weights be in a range of
  // (-1 * item_count) to (item_count). This means that when we remove one,
  // the range shrinks; weights outside of that range then get set to
  // the first item in the select by the browser, floating them to the top.
  // We use a brute force method because we lost weights on both ends
  // and if the user has moved things around, we have to cascade because
  // if I have items weight weights 3 and 4, and I change 4 to 3 but leave
  // the 3, the order of the two 3s now is undefined and may not match what
  // the user had selected.
  $input = drupal_array_get_nested_value($form_state['input'], $address);
  // Sort by weight,
  // but first remove garbage values to ensure proper '_weight' sorting
  unset($input['add_more']);
  uasort($input, '_field_sort_items_helper');

  // Reweight everything in the correct order.
  $weight = -1 * $field_state['items_count'] + 1;
  foreach ($input as $key => $item) {
    if ($item) {
      $input[$key]['_weight'] = $weight++;
    }
  }

  drupal_array_set_nested_value($form_state['input'], $address, $input);
  field_form_set_state($parents, $field_name, $langcode, $form_state, $field_state);

  $form_state['rebuild'] = TRUE;
}

/**
 * Submit callback to editing an item from the field UI multiple wrapper.
 *
 * When a edited button is submitted, we need to find the item that it
 * referenced and delete it. Since field UI has the deltas as a straight
 * unbroken array key, we have to renumber everything down. Since we do this
 * we *also* need to move all the deltas around in the $form_state['values']
 * and $form_state['input'] so that user changed values follow. This is a bit
 * of a complicated process.
 */
function paragraphs_edit_submit($form, &$form_state) {
  $button = $form_state['triggering_element'];
  $delta = $button['#delta'];

  // Where in the form we'll find the parent element.
  $address = array_slice($button['#array_parents'], 0, -3);

  // Go one level up in the form, to the widgets container.
  $parent_element = drupal_array_get_nested_value($form, $address);
  $field_name = $parent_element['#field_name'];
  $langcode = $parent_element['#language'];
  $parents = $parent_element['#field_parents'];

  $field_state = field_form_get_state($parents, $field_name, $langcode, $form_state);

  if (isset($field_state['entity'][$delta])) {
    $field_state['entity'][$delta]->being_edited = 1;
  }

  // Fix the weights. Field UI lets the weights be in a range of
  // (-1 * item_count) to (item_count). This means that when we remove one,
  // the range shrinks; weights outside of that range then get set to
  // the first item in the select by the browser, floating them to the top.
  // We use a brute force method because we lost weights on both ends
  // and if the user has moved things around, we have to cascade because
  // if I have items weight weights 3 and 4, and I change 4 to 3 but leave
  // the 3, the order of the two 3s now is undefined and may not match what
  // the user had selected.
  $input = drupal_array_get_nested_value($form_state['input'], $address);
  // Sort by weight,
  // but first remove garbage values to ensure proper '_weight' sorting
  unset($input['add_more']);
  uasort($input, '_field_sort_items_helper');

  // Reweight everything in the correct order.
  $weight = -1 * $field_state['items_count'] + 1;
  foreach ($input as $key => $item) {
    if ($item) {
      $input[$key]['_weight'] = $weight++;
    }
  }

  drupal_array_set_nested_value($form_state['input'], $address, $input);
  field_form_set_state($parents, $field_name, $langcode, $form_state, $field_state);

  $form_state['rebuild'] = TRUE;
}

/**
 * Submit callback to remove an item from the field UI multiple wrapper.
 *
 * When a remove button is submitted, we need to find the item that it
 * referenced and delete it. Since field UI has the deltas as a straight
 * unbroken array key, we have to renumber everything down. Since we do this
 * we *also* need to move all the deltas around in the $form_state['values']
 * and $form_state['input'] so that user changed values follow. This is a bit
 * of a complicated process.
 */
function paragraphs_deleteconfirm_submit($form, &$form_state) {
  $button = $form_state['triggering_element'];
  $delta = $button['#delta'];

  // Where in the form we'll find the parent element.
  $address = array_slice($button['#array_parents'], 0, -3);

  // Go one level up in the form, to the widgets container.
  $parent_element = drupal_array_get_nested_value($form, $address);
  $field_name = $parent_element['#field_name'];
  $langcode = $parent_element['#language'];
  $parents = $parent_element['#field_parents'];

  $field_state = field_form_get_state($parents, $field_name, $langcode, $form_state);

  // Go ahead and renumber everything from our delta to the last
  // item down one. This will overwrite the item being removed.
  for ($i = $delta; $i <= $field_state['items_count']; $i++) {
    $old_element_address = array_merge($address, array($i + 1));
    $new_element_address = array_merge($address, array($i));

    $moving_element = drupal_array_get_nested_value($form, $old_element_address);
    $moving_element_value = drupal_array_get_nested_value($form_state['values'], $old_element_address);
    $moving_element_input = drupal_array_get_nested_value($form_state['input'], $old_element_address);

    // Tell the element where it's being moved to.
    $moving_element['#parents'] = $new_element_address;

    // Move the element around.
    form_set_value($moving_element, $moving_element_value, $form_state);
    drupal_array_set_nested_value($form_state['input'], $moving_element['#parents'], $moving_element_input);

    // Move the entity in our saved state.
    if (isset($field_state['entity'][$i + 1])) {
      $field_state['entity'][$i] = $field_state['entity'][$i + 1];
    }
    else {
      unset($field_state['entity'][$i]);
    }
  }

  // Replace the deleted entity with an empty one. This helps to ensure that
  // trying to add a new entity won't ressurect a deleted entity from the
  // trash bin.
  $count = count($field_state['entity']);
  unset($field_state['entity'][$count]);

  $field_state['removed_one'] = TRUE;

  // Then remove the last item. But we must not go negative.
  if ($field_state['items_count'] > 0) {
    $field_state['items_count']--;
  }

  // Fix the weights. Field UI lets the weights be in a range of
  // (-1 * item_count) to (item_count). This means that when we remove one,
  // the range shrinks; weights outside of that range then get set to
  // the first item in the select by the browser, floating them to the top.
  // We use a brute force method because we lost weights on both ends
  // and if the user has moved things around, we have to cascade because
  // if I have items weight weights 3 and 4, and I change 4 to 3 but leave
  // the 3, the order of the two 3s now is undefined and may not match what
  // the user had selected.
  $input = drupal_array_get_nested_value($form_state['input'], $address);
  // Sort by weight,
  // but first remove garbage values to ensure proper '_weight' sorting
  unset($input['add_more']);
  uasort($input, '_field_sort_items_helper');

  // Reweight everything in the correct order.
  $weight = -1 * $field_state['items_count'] + 1;
  foreach ($input as $key => $item) {
    if ($item) {
      $input[$key]['_weight'] = $weight++;
    }
  }

  drupal_array_set_nested_value($form_state['input'], $address, $input);
  field_form_set_state($parents, $field_name, $langcode, $form_state, $field_state);

  $form_state['rebuild'] = TRUE;
}

/**
 * Submit function to restore a paragraph that was deleted.
 * @param $form
 * @param $form_state
 */
function paragraphs_restore_submit($form, &$form_state) {
  $button = $form_state['triggering_element'];
  $delta = $button['#delta'];

  // Where in the form we'll find the parent element.
  $address = array_slice($button['#array_parents'], 0, -3);

  // Go one level up in the form, to the widgets container.
  $parent_element = drupal_array_get_nested_value($form, $address);
  $field_name = $parent_element['#field_name'];
  $langcode = $parent_element['#language'];
  $parents = $parent_element['#field_parents'];

  $field_state = field_form_get_state($parents, $field_name, $langcode, $form_state);


  if (isset($field_state['entity'][$delta])) {
    $field_state['entity'][$delta]->removed = 0;
  }


  // Fix the weights. Field UI lets the weights be in a range of
  // (-1 * item_count) to (item_count). This means that when we remove one,
  // the range shrinks; weights outside of that range then get set to
  // the first item in the select by the browser, floating them to the top.
  // We use a brute force method because we lost weights on both ends
  // and if the user has moved things around, we have to cascade because
  // if I have items weight weights 3 and 4, and I change 4 to 3 but leave
  // the 3, the order of the two 3s now is undefined and may not match what
  // the user had selected.
  $input = drupal_array_get_nested_value($form_state['input'], $address);
  // Sort by weight,
  // but first remove garbage values to ensure proper '_weight' sorting
  unset($input['add_more']);
  uasort($input, '_field_sort_items_helper');

  // Reweight everything in the correct order.
  $weight = -1 * $field_state['items_count'] + 1;
  foreach ($input as $key => $item) {
    if ($item) {
      $input[$key]['_weight'] = $weight++;
    }
  }

  drupal_array_set_nested_value($form_state['input'], $address, $input);
  field_form_set_state($parents, $field_name, $langcode, $form_state, $field_state);

  $form_state['rebuild'] = TRUE;
}
