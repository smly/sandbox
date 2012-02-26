#!perl -w
use 5.14.0;
package Point {
    use Mouse;

    has [qw(x y)] => (
        is       => 'rw',
        isa      => 'Num',
        required => 1,
    );

    __PACKAGE__->meta->make_immutable();
}

say Point->new( x => 10, y => 20 )->dump();
__END__
