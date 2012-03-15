require 'formula'

class ReadText < Formula
  head 'https://github.com/dtcristo/read_text.git'
  homepage 'https://github.com/dtcristo/read_text'

  depends_on 'tesseract'
  depends_on 'opencv'
  depends_on 'boost'

  def install
    system "make"
    bin.install "read_text"
    bin.install "scripts/read_text_batch.py"
  end
end
