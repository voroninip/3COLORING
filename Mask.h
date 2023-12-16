#include <cassert>
#include <unordered_set>

using std::unordered_set;
using vertex = int;

struct Mask {
  int mask = 0;

  Mask() = default;

  explicit Mask(int x): mask(x) {} // simply interpret int value as a mask

  explicit operator int() const {
    return mask;
  }

  bool empty() const {
    return mask == 0;
  }

  Mask& operator+=(Mask other) {
    mask += other.mask;
    return *this;
  }
};

Mask operator and(Mask A, Mask B) { // A \cap B
  return Mask(A.mask & B.mask);
}

Mask operator *(Mask A, Mask B) { // A \cap B
  return Mask(A.mask & B.mask);
}

Mask operator or(Mask A, Mask B) { // A \cup B
  return Mask(A.mask | B.mask);
}

Mask operator +(Mask A, Mask B) { // A \cup B
  return Mask(A.mask | B.mask);
}

Mask operator xor(Mask A, Mask B) { // bitwise xor
  return Mask(A.mask xor B.mask);
}

bool operator ==(Mask A, Mask B) {
  return A.mask == B.mask;
}

bool operator !=(Mask A, Mask B) {
  return A.mask != B.mask;
}

bool operator <=(Mask A, Mask B) { // A \subseteq B
  return (A and B) == A;
}

bool operator <(Mask A, Mask B) { // A \subsetneq B
  return (A <= B) and (A != B);
}

bool operator >(Mask A, Mask B) { // A \supsetneq B
  return B < A;
}

bool operator >=(Mask A, Mask B) { // A \supseteq B
  return B <= A;
}

Mask operator -(Mask A, Mask B) { // set difference
  assert(A >= B);
  return (A or B) xor B;
}

Mask create_mask(vertex bit) { // mask of one vertex {v}
  return Mask(1 << (bit - 1));
}

Mask create_mask(const unordered_set<vertex>& bits) { // {v1, v2, ..., v_m}
  if (bits.empty()) {
    return Mask(0);
  }
  Mask mask = Mask(0);
  for (int bit : bits) {
    mask += Mask(1 << (bit - 1));
  }
  return mask;
}