  result res;
  res.count = length;
  res.error = error_code::SUCCESS;
  for (size_t i = 0; i < length; ++i) {
    output[i] = input[i];
  }
  return res;
}
