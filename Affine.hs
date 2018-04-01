import System.Environment

encrypt :: Int -> Int -> String -> String
encrypt = map encrypt'
  where
    encrypt' a b c

decrypt :: Int -> Int -> String -> String
decrypt =  map decrypt'
  where
    decrypt' a b c

affine :: [String] -> String
affine (arg1 : arg2 : arg3 : msg) =
  if arg1 == "-d"
     then decrypt (reads arg2) (read arg3) (unwords msg)
  else encrypt (read arg1) (read arg 2) (unwords (arg3 : msg))

main = getArgs >>= putStrLn . affine
