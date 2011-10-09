<?php

/**
 * Class that extends SimpleXMLElement to add some parsing helpers.
 */
class CultureFeed_SimpleXMLElement extends SimpleXMLElement {

  /**
   * Runs XPath query on XML data and casts it to a string or an array of string values.
   * @see xpath_cast for more documentation on the arguments.
   */
  public function xpath_str($path, $multiple = FALSE, $trim = TRUE) {
    $tmp = $this->xpath_cast('strval', $path, $multiple);
    if ($tmp && !$multiple && $trim) {
      return trim($tmp);
    }
    return $tmp;
  }

  /**
   * Runs XPath query on XML data and casts it to an integer or an array of integer values.
   * @see xpath_cast for more documentation on the arguments.
   */
  public function xpath_int($path, $multiple = FALSE) {
    return $this->xpath_cast('intval', $path, $multiple);
  }

  /**
   * Runs XPath query on XML data and casts it to a float or an array of float values.
   * @see xpath_cast for more documentation on the arguments.
   */
  public function xpath_float($path, $multiple = FALSE) {
    return $this->xpath_cast('floatval', $path, $multiple);
  }

  /**
   * Runs XPath query on XML data and casts it to a UNIX timestamp or an array of timestamps.
   * @see xpath_cast for more documentation on the arguments.
   */
  public function xpath_time($path, $multiple = FALSE) {
    $val = $this->xpath_cast('strval', $path, $multiple);
    if (!$val) {
      return NULL;
    }

    if ($multiple) {
      foreach ($val as $key => $value) {
        $val[$key] = strtotime($value);
      }
    }

    return strtotime($val);
  }

  /**
   * Runs XPath query on XML data and casts it to a bool or an array of bool values.
   * @see xpath_cast for more documentation on the arguments.
   */
  public function xpath_bool($path, $multiple = FALSE) {
    $val = $this->xpath_cast('strval', $path, $multiple);
    if (!$val) {
      return NULL;
    }

    if ($multiple) {
      foreach ($val as $key => $value) {
        $val[$key] = strtolower($value) == 'true' ? TRUE : FALSE;
      }
    }

    return strtolower($val) == 'true' ? TRUE : FALSE;
  }

  /**
   * Runs XPath query on XML data and casts it using a type casting function.
   *
   * @param string $cast_function
   * @param string $path
   *   The XPath query.
   * @param string $multiple
   *   Does the query direct to a path where multiple values are possible?
   * @return array|undefined
   *   In case $multiple is TRUE, an array is returned with the type casted elements.
   *   In case $multiple is FALSE, the result of the XPath query is casted using the $cast_function and type depends on type of that function.
   *   In case no nodes were found with the query, NULL is returned.
   */
  private function xpath_cast($cast_function, $path, $multiple = FALSE) {
    $objects = $this->xpath($path);

    if (!is_array($objects)) return $objects;

    if ($multiple) {
      $result = array();
      foreach ($objects as $object) {
        $result[] = is_null($object) || ($cast_function != 'strval' && empty($object)) ? NULL : call_user_func($cast_function, $object);
      }
      return array_filter($result);
    }
    else {
      return empty($objects) || is_null($objects[0]) || ($cast_function != 'strval' && empty($objects[0])) ? NULL : call_user_func($cast_function, $objects[0]);
    }
  }

}