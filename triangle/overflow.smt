(declare-const edge1 Int)
(declare-const edge2 Int)
(declare-const edge3 Int)

(assert (>= edge1 0))
(assert (>= edge2 0))
(assert (>= edge3 0))

(assert (<= edge1 4611686014132420609))
(assert (<= edge2 4611686014132420609))
(assert (<= edge3 4611686014132420609))

; Assert that two edges are able to overflow *slightly*.
(assert (>= edge1 536870912 ))
(assert (<= edge1 1073741823))
(assert (>= edge2 536870912 ))
(assert (<= edge2 1073741823))

; Assert that these two edges are the smaller of the two edges so they will
; be added together.
(assert (< edge1 edge3))
(assert (< edge2 edge3))

(declare-const x1 Int)
(declare-const x2 Int)
(declare-const x3 Int)

(declare-const y1 Int)
(declare-const y2 Int)
(declare-const y3 Int)

; Assert that xs and ys are greater than or equal to 0
(assert (>= x1 0))
(assert (>= x2 0))
(assert (>= x3 0))
(assert (>= y1 0))
(assert (>= y2 0))
(assert (>= y3 0))

; Assert that xs and ys are less than or equal to (2 ** 31) - 1
(assert (<= x1 2147483647))
(assert (<= x2 2147483647))
(assert (<= x3 2147483647))
(assert (<= y1 2147483647))
(assert (<= y2 2147483647))
(assert (<= y3 2147483647))

(assert (= edge1 (+ (* (- x2 x1) (- x2 x1))
                    (* (- y2 y1) (- y2 y1)))))

(assert (= edge2 (+ (* (- x3 x2) (- x3 x2))
                    (* (- y3 y2) (- y3 y2)))))

(assert (= edge3 (+ (* (- x1 x3) (- x1 x3))
                    (* (- y1 y3) (- y1 y3)))))

(check-sat)
(get-model)
